#include <Arduino.h>

// Настройки (обязательно зайди)
#include "config.h"

// Библиотеки
#include <EEManager.h>
#include <EncButton.h>
#include <GyverSegment.h>
#include <GyverNTC.h>
#include <TimerMs.h>
#include <PinChangeInterrupt.h>

// Переменные

int8_t readed_temp = DEFAULT_READED_TEMP;
bool need_redraw_display = false;
uint8_t mode = MODE_DEFAULT;
uint8_t error_code = NO_ERROR;
uint8_t message_code;

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
  if (readed_temp < settings.setted_temp - settings.hysteresis)
    digitalWrite(PIN_RELAY, HIGH);
  else if (readed_temp > settings.setted_temp + settings.hysteresis)
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
    print_mode("S", settings.setted_temp);
  else if (mode == MODE_READED_TEMP)
    print_mode("C", readed_temp);
  else if (mode == MODE_HYSTERESIS)
    print_mode("H", settings.hysteresis);
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
    if (mode == MODE_SETTED_TEMP)
    {
      settings.setted_temp += TEMP_SET_STEP * ENCODER_CHANGE_DIR * enc.dir();
      settings.setted_temp = constrain(settings.setted_temp, MIN_SETTED_TEMP, MAX_SETTED_TEMP);
    }
    else if (mode == MODE_HYSTERESIS)
    {
      settings.hysteresis += HYSTERESIS_SET_STEP * ENCODER_CHANGE_DIR * enc.dir();
      settings.hysteresis = constrain(settings.hysteresis, MIN_HYSTERESIS, MAX_HYSTERESIS);
    }

    DEBUG("enc_handle: ");
    DEBUG("setted_temp is ");
    DEBUG(setted_temp);
    DEBUG(", hysteresis is ");
    DEBUG(hysteresis);
    DEBUGLN("");

    memory.update();
    need_redraw_display = true;
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

  memory.begin(EEPROM_SETTINGS_ADDR, EEPROM_INIT_MARKER_VALUE);

  need_redraw_display = true;
}

void loop()
{
  if (memory.tick())
  {
    message_code = MESSAGE_SAVED;
    show_message();
  }

  enc.tick();

  if (temp_read_timer.tick())
    read_and_show_temp();

  if (relay_update_timer.tick())
    update_relay();

  if (message_close_timer.tick())
    close_message();

  if (error_code && mode != MODE_ERROR)
    show_error();

  enc_handle();

  if (need_redraw_display)
    redraw_display();
}
