// Настройки (обязательно зайди)
#include "config.h"

// Библиотеки
#include <EEPROM.h>
#include <EncButton.h>
#include <GyverSegment.h>
#include <GyverNTC.h>
#include <TimerMs.h>
#include <PinChangeInterrupt.h>

// Переменныеs
int8_t setted_temp = DEFAULT_SETTED_TEMP;
int8_t readed_temp = DEFAULT_READED_TEMP;
int8_t hysteresis = DEFAULT_HYSTERESIS;
bool need_redraw_display = false;
uint8_t mode = MODE_DEFAULT;
uint32_t last_encoder_event;
bool need_save_settings = false;
uint8_t error_code = NO_ERROR;
uint8_t message_code;

// Инициализация объектов
EncButton enc(PIN_ENC_A, PIN_ENC_B, PIN_ENC_BUTT);
Disp1637Colon display(PIN_TM1637_DIO, PIN_TM1637_CLK);
GyverNTC ntc(PIN_NTC, NTC_RESISTOR, NTC_BETA_COEF, NTC_BASE_TEMP, NTC_BASE_RESISTANCE, NTC_RESOLUTION);
TimerMs temp_read_timer(TEMP_READ_TIME, 1, 0);
TimerMs relay_update_timer(RELAY_UPDATE_TIME, 1, 0);
TimerMs save_settings_timer(SETTINGS_SAVE_TIME, 0, 1);
TimerMs message_close_timer(MESSAGE_TIME, 0, 1);

void show_error()
{
  mode = MODE_ERROR;

  need_redraw_display = true;
}

void show_message()
{
  mode = MODE_MESSAGE;

  message_close_timer.start();

  need_redraw_display = true;

  DEBUGLN("show_message: showed");
}

void close_message()
{
  mode = MODE_DEFAULT;

  need_redraw_display = true;

  DEBUGLN("close_message: closed");
}

void save_settings()
{
  DEBUG("save_settings: ");
  DEBUG("setted_temp is ");
  DEBUG(setted_temp);
  DEBUGLN("");

  DEBUG("save_settings: ");
  DEBUG("hysteresis is ");
  DEBUG(hysteresis);
  DEBUGLN("");

  EEPROM.put(EEPROM_SETTED_TEMP_ADDR, setted_temp);
  EEPROM.put(EEPROM_HYSTERESIS_ADDR, hysteresis);

  message_code = MESSAGE_SAVED;
  show_message();

  need_save_settings = false;
}

void read_and_show_temp()
{
  int8_t previous_readed_temp = readed_temp;
  readed_temp = ntc.getTempAverage();

  DEBUG("read_and_show_temp: ");
  DEBUG("readed_temp is ");
  DEBUG(readed_temp);
  DEBUGLN("");

  if (previous_readed_temp != readed_temp && mode == MODE_READED_TEMP)
    need_redraw_display = true;
}

void update_relay()
{
  if (readed_temp < setted_temp - hysteresis)
    digitalWrite(PIN_RELAY, HIGH);
  else if (readed_temp > setted_temp + hysteresis)
    digitalWrite(PIN_RELAY, LOW);
}

void change_mode()
{
  mode++;
  if (mode > MODE_SWITCH_MAX)
    mode = MODE_SWITCH_MIN;

  need_redraw_display = true;
}

void put_num_at_end(int32_t num)
{
  int num_len = sseg::intLen(num);
  display.setCursor(TM1637_DIGITS_NUM - num_len);
  display.print(num);
}

void print_mode(String mode_name, int32_t num)
{
  display.print(mode_name);
  put_num_at_end(num);
}

void redraw_display()
{
  DEBUG("redraw_display: ");
  DEBUG("redraw");
  DEBUGLN("");

  display.setCursor(0);
  display.clear();

  if (mode == MODE_SETTED_TEMP)
    print_mode("S", setted_temp);
  else if (mode == MODE_READED_TEMP)
    print_mode("C", readed_temp);
  else if (mode == MODE_HYSTERESIS)
    print_mode("H", hysteresis);
  else if (mode == MODE_ERROR)
    print_mode("E", error_code);
  else if (mode == MODE_MESSAGE)
    print_mode("I", message_code);
  else
    error_code = DISPLAY_ERROR;

  display.update();

  need_redraw_display = false;
}

void enc_handle()
{
  if (enc.click())
    change_mode();
  if (enc.turn())
  {
    int8_t previous_setted_temp = setted_temp;
    int8_t previous_hysteresis = hysteresis;
    if (mode == MODE_SETTED_TEMP)
    {
      setted_temp += TEMP_SET_STEP * ENCODER_CHANGE_DIR * enc.dir();
      setted_temp = constrain(setted_temp, MIN_SETTED_TEMP, MAX_SETTED_TEMP);
    }
    else if (mode == MODE_HYSTERESIS)
    {
      hysteresis += HYSTERESIS_SET_STEP * ENCODER_CHANGE_DIR * enc.dir();
      hysteresis = constrain(hysteresis, MIN_HYSTERESIS, MAX_HYSTERESIS);
    }

    DEBUG("enc_handle: ");
    DEBUG("setted_temp is ");
    DEBUG(setted_temp);
    DEBUG(", hysteresis is ");
    DEBUG(hysteresis);
    DEBUGLN("");

    if (previous_setted_temp != setted_temp || previous_hysteresis != hysteresis)
    {
      need_save_settings = true;
      need_redraw_display = true;
      save_settings_timer.start();
    }
  }
}

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

  // Если запустили первый раз
  if (EEPROM.read(EEPROM_INIT_MARKER_ADDR) != EEPROM_INIT_MARKER_VALUE)
  {
    EEPROM.write(EEPROM_INIT_MARKER_ADDR, EEPROM_INIT_MARKER_VALUE); // Пометили что запустили

    EEPROM.put(EEPROM_SETTED_TEMP_ADDR, setted_temp);
    EEPROM.put(EEPROM_HYSTERESIS_ADDR, hysteresis);
  }

  EEPROM.get(EEPROM_SETTED_TEMP_ADDR, setted_temp);
  EEPROM.get(EEPROM_HYSTERESIS_ADDR, hysteresis);

  need_redraw_display = true;
}

void loop()
{
  enc.tick();

  enc_handle();

  if (temp_read_timer.tick())
    read_and_show_temp();

  if (relay_update_timer.tick())
    update_relay();

  if (save_settings_timer.tick() && need_save_settings)
    save_settings();

  if (error_code && mode != MODE_ERROR)
    show_error();

  if (message_close_timer.tick())
    close_message();

  if (need_redraw_display)
    redraw_display();
}
