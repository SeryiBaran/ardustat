#include "main.h"

// Переменные
int16_t sens_temp = AS_DEFAULT_SENS_TEMP;
bool need_redraw_display = false;
uint8_t mode = AS_MODE_DEFAULT;
uint8_t message_code = AS_MESSAGE_NO;
bool message_is_error = false;

// Изменяемые настройки
struct Settings
{
  int16_t temp = AS_DEFAULT_TEMP;
  int16_t hstr = AS_DEFAULT_HSTR;
};

Settings settings;

// Инициализация объектов
EEManager memory(settings, AS_SETTINGS_SAVE_TIME);
EncButton enc(AS_PIN_ENC_A, AS_PIN_ENC_B, AS_PIN_ENC_BUTT);
Disp1637Colon display(AS_PIN_TM1637_DIO, AS_PIN_TM1637_CLK);
GyverNTC ntc(AS_PIN_NTC, AS_NTC_RESISTOR, AS_NTC_BETA_COEF, AS_NTC_BASE_TEMP, AS_NTC_BASE_RESISTANCE, AS_NTC_RESOLUTION);
TimerMs temp_read_timer(AS_TEMP_READ_TIME, 1, 0);
TimerMs relay_update_timer(AS_RELAY_UPDATE_TIME, 1, 0);
TimerMs message_close_timer(AS_MESSAGE_TIME, 0, 1);

// Планируем отобразить сообщение при следующей отрисовке дисплея
void add_message()
{
  mode = AS_MODE_MESSAGE;

  message_close_timer.start();

  need_redraw_display = true;
}

// Планируем убрать сообщение при следующей отрисовке дисплея
void remove_message()
{
  mode = AS_MODE_DEFAULT;

  need_redraw_display = true;
}

// Читаем температуру
void read_temp()
{
  sens_temp = ntc.getTempAverage();
}

// Обновляем состояние реле
void update_relay()
{
  // Гистерезис - https://alexgyver.ru/lessons/relay-control/
  if (sens_temp < (settings.temp - settings.hstr))
    digitalWrite(AS_PIN_RELAY, HIGH);
  else if (sens_temp > (settings.temp + settings.hstr))
    digitalWrite(AS_PIN_RELAY, LOW);
}

// Меняем режим
void change_mode()
{
  mode++;
  if (mode > AS_MODE_SWITCH_MAX)
    mode = AS_MODE_SWITCH_MIN;

  need_redraw_display = true;
}

// Печатаем режим (его символ и число) (но не обновляем дисплей!)
void print_mode(const char *mode_symbol, int16_t num)
{
  display.print(mode_symbol);

  // Печатаем число в конце экрана
  uint8_t num_len = sseg::intLen(num);
  display.setCursor(AS_TM1637_DIGITS_NUM - num_len);
  display.print(num);
}

// Обновление дисплея
void redraw_display()
{
  display.setCursor(0);
  display.clear();

  if (mode == AS_MODE_TEMP)
    print_mode(AS_MODE_SYMBOL_TEMP, settings.temp);
  else if (mode == AS_MODE_SENS_TEMP)
    print_mode(AS_MODE_SYMBOL_SENS_TEMP, sens_temp);
  else if (mode == AS_MODE_HSTR)
    print_mode(AS_MODE_SYMBOL_HSTR, settings.hstr);
  else if (mode == AS_MODE_MESSAGE)
  {
    // Если сообщение имеет ID ошибки, выводим символ ошибки. Иначе символ сообщения
    print_mode(message_is_error
                   ? AS_MODE_SYMBOL_ERROR
                   : AS_MODE_SYMBOL_MESSAGE,
               message_code);

    // Вывели - чистим за собой
    message_code = AS_MESSAGE_NO;
    message_is_error = false;
  }
  else
  {
    // Если ID режима не подходит, ставим ошибку. При следующем рендере она покажется
    message_code = AS_MESSAGE_ERROR_DISPLAY;
    message_is_error = true;
  }

  display.update();

  need_redraw_display = false;
}

// Работа с энкодером
void enc_handle()
{
  enc.tick();

  if (enc.click())
    change_mode();

  if (enc.turn())
  {
    // `-1` - шаг изменения, указывается отрицательным числом
    if (mode == AS_MODE_TEMP)
      settings.temp = constrain(settings.temp + -1 * enc.dir(), AS_MIN_TEMP, AS_MAX_TEMP);
    else if (mode == AS_MODE_HSTR)
      settings.hstr = constrain(settings.hstr + -1 * enc.dir(), AS_MIN_HSTR, AS_MAX_HSTR);

    memory.update();
    need_redraw_display = true;
  }
}

// Функция обработки программного прерывания для энкодера. Читайте https://github.com/GyverLibs/EncButton
void enc_isr()
{
  enc.tickISR();
}

void setup()
{
#ifdef ENABLE_DEBUG
  Serial.begin(115200);
#endif

  attachPCINT(digitalPinToPCINT(AS_PIN_ENC_A), enc_isr, CHANGE);
  attachPCINT(digitalPinToPCINT(AS_PIN_ENC_B), enc_isr, CHANGE);
  attachPCINT(digitalPinToPCINT(AS_PIN_ENC_BUTT), enc_isr, CHANGE);

  memory.begin(AS_EEPROM_SETTINGS_ADDR, AS_EEPROM_INIT_MARKER_VALUE);

  need_redraw_display = true;
}

void loop()
{
  // Если настройки были записаны в EEPROM - ставим радостное сообщение
  if (memory.tick())
    message_code = AS_MESSAGE_SAVED;

  if (temp_read_timer.tick())
  {
    read_temp();
    need_redraw_display = true;
  }

  enc_handle();

  if (relay_update_timer.tick())
    update_relay();

  if (message_code)
    add_message();
  if (message_close_timer.tick())
    remove_message();
  if (need_redraw_display)
    redraw_display();
}
