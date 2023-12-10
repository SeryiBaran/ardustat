#include "main.h"

// Переменные
int8_t readed_temp = DEFAULT_READED_TEMP;
bool need_redraw_display = false;
uint8_t mode = MODE_DEFAULT;
uint8_t message_code = MESSAGE_NO;

// Изменяемые настройки
struct Settings
{
  int8_t setted_temp = DEFAULT_SETTED_TEMP;
  int8_t hysteresis = DEFAULT_HYSTERESIS;
};

Settings settings;

// Инициализация объектов
EEManager memory(settings, SETTINGS_SAVE_TIME);
EncButton enc(PIN_ENC_A, PIN_ENC_B, PIN_ENC_BUTT);
Disp1637Colon display(PIN_TM1637_DIO, PIN_TM1637_CLK);
GyverNTC ntc(PIN_NTC, NTC_RESISTOR, NTC_BETA_COEF, NTC_BASE_TEMP, NTC_BASE_RESISTANCE, NTC_RESOLUTION);
TimerMs temp_read_timer(TEMP_READ_TIME, 1, 0);
TimerMs relay_update_timer(RELAY_UPDATE_TIME, 1, 0);
TimerMs message_close_timer(MESSAGE_TIME, 0, 1);

// Планируем отобразить сообщение при следующей отрисовке дисплея
void add_message()
{
  mode = MODE_MESSAGE;

  message_close_timer.start();

  need_redraw_display = true;

  DEBUGLN(F("add_message: showed"));
}

// Планируем убрать сообщение при следующей отрисовке дисплея
void remove_message()
{
  mode = MODE_DEFAULT;

  need_redraw_display = true;

  DEBUGLN(F("remove_message: closed"));
}

// Читаем температуру
void read_temp()
{
  readed_temp = ntc.getTempAverage();
}

// Обновляем состояние реле
void update_relay()
{
  if (readed_temp < settings.setted_temp - settings.hysteresis)
    digitalWrite(PIN_RELAY, HIGH);
  else if (readed_temp > settings.setted_temp + settings.hysteresis)
    digitalWrite(PIN_RELAY, LOW);
}

// Меняем режим
void change_mode()
{
  mode++;
  if (mode > MODE_SWITCH_MAX)
    mode = MODE_SWITCH_MIN;

  need_redraw_display = true;
}

// Печатаем число в конце экрана (но не обновляем дисплей!)
void put_num_at_end(int32_t num)
{
  int num_len = sseg::intLen(num);
  display.setCursor(TM1637_DIGITS_NUM - num_len);
  display.print(num);
}

// Печатаем режим (его символ и число) (но не обновляем дисплей!)
void print_mode(String mode_name, int32_t num)
{
  display.print(mode_name);
  put_num_at_end(num);
}

// Обновление дисплея
void redraw_display()
{
  DEBUGLN(F("redraw_display: redraw"));

  display.setCursor(0);
  display.clear();

  if (mode == MODE_SETTED_TEMP)
    print_mode(MODE_SYMBOL_SETTED_TEMP, settings.setted_temp);
  else if (mode == MODE_READED_TEMP)
    print_mode(MODE_SYMBOL_READED_TEMP, readed_temp);
  else if (mode == MODE_HYSTERESIS)
    print_mode(MODE_SYMBOL_HYSTERESIS, settings.hysteresis);
  else if (mode == MODE_MESSAGE)
    print_mode(message_code >= MESSAGE_ERROR_FIRST
                   ? MODE_SYMBOL_ERROR
                   : MODE_SYMBOL_MESSAGE,
               message_code); // Если сообщение имеет ID ошибки, выводим символ ошибки. Иначе символ сообщения
  else
    message_code = MESSAGE_ERROR_DISPLAY; // Если ID режима не подходит, ставим ошибку. При следующем рендере она покажется

  display.update();

  need_redraw_display = false;

  // TODO: перенести в IF
  message_code = MESSAGE_NO; // Вывели сообщение - очищаем код сообщения чтобы заново не показалось
}

// Работа с энкодером
void enc_handle()
{
  enc.tick();

  if (enc.click())
    change_mode();

  if (enc.turn())
  {
    if (mode == MODE_SETTED_TEMP)
    {
      settings.setted_temp += TEMP_SET_STEP * ENCODER_CHANGE_DIR * enc.dir(); // Немножко несложной магии. Читайте https://github.com/GyverLibs/EncButton
      settings.setted_temp = constrain(settings.setted_temp, MIN_SETTED_TEMP, MAX_SETTED_TEMP);
    }
    else if (mode == MODE_HYSTERESIS)
    {
      settings.hysteresis += HYSTERESIS_SET_STEP * ENCODER_CHANGE_DIR * enc.dir();
      settings.hysteresis = constrain(settings.hysteresis, MIN_HYSTERESIS, MAX_HYSTERESIS);
    }

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

  attachPCINT(digitalPinToPCINT(PIN_ENC_A), enc_isr, CHANGE);
  attachPCINT(digitalPinToPCINT(PIN_ENC_B), enc_isr, CHANGE);
  attachPCINT(digitalPinToPCINT(PIN_ENC_BUTT), enc_isr, CHANGE);

  memory.begin(EEPROM_SETTINGS_ADDR, EEPROM_INIT_MARKER_VALUE);

  need_redraw_display = true;
}

void loop()
{
  // Если настройки были записаны в EEPROM - ставим радостное сообщение
  if (memory.tick())
  {
    message_code = MESSAGE_SAVED;
    add_message();
  }

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
