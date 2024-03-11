#pragma once

// Раскомментируй для отладки по Serial
#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define DEBUG(x) Serial.print(x)
#define DEBUGLN(x) Serial.println(x)
#else
#define DEBUG(x)
#define DEBUGLN(x)
#endif

// ======== Пины

// Пин термистора
#define AS_PIN_NTC A0
// Пин реле
#define AS_PIN_RELAY 2
// Пин энкодера A
#define AS_PIN_ENC_A 5
// Пин энкодера B
#define AS_PIN_ENC_B 4
// Пин кнопки энкодера
#define AS_PIN_ENC_BUTT 3
// Пин CLK дисплея TM1637
#define AS_PIN_TM1637_CLK 11
// Пин DIO дисплея TM1637
#define AS_PIN_TM1637_DIO 12

// ======== Разные настройки

// Минимальная температура
#define AS_MIN_TEMP 20
// Максимальная температура
#define AS_MAX_TEMP 60
// Температура по умолчанию
#define AS_DEFAULT_TEMP 20
// Температура с датчика по умолчанию
#define AS_DEFAULT_SENS_TEMP 99
// Мин. гистерезис
#define AS_MIN_HSTR 1
// Макс. гистерезис
#define AS_MAX_HSTR 10
// Гистерезис по умолчанию
#define AS_DEFAULT_HSTR 2

// ======== Таймауты (в миллисекундах)

// Таймаут замера температура
#define AS_TEMP_READ_TIME 500
// Время между переключениями реле
#define AS_RELAY_UPDATE_TIME 3000
// Время показа сообщения
#define AS_MESSAGE_TIME 3000
// Задержка сохранения настроек
#define AS_SETTINGS_SAVE_TIME 5000
// Задержка выключения дисплея
#define AS_DISPLAY_OFF_TIME 15000

// ======== Настройки EEPROM

#define AS_EEPROM_INIT_MARKER_VALUE \
  10  // Значение флага по которому устройство будет понимать что уже включалось
      // и в нем что-то сохранено. Взято от балды, но не должно быть
      // равно стандартному значению в EEPROM `255`

#define AS_EEPROM_SETTINGS_ADDR 0  // Адрес с настройками

// ======== Настройки дисплея

// Количество разрядов на дисплее
#define AS_TM1637_DIGITS_NUM 4

// ======== Режимы

// Заданная температура
#define AS_MODE_TEMP 0
// Текущая температура
#define AS_MODE_SENS_TEMP 1
// Гистерезис
#define AS_MODE_HSTR 2
// Сообщение
#define AS_MODE_MESSAGE 101
// Выключенный экран
#define AS_MODE_DISPLAY_OFF 102
// Стандартный режим
#define AS_MODE_DEFAULT AS_MODE_TEMP
// Первый режим доступный для открытия
#define AS_MODE_SWITCH_MIN 0
// Последний режим доступный для открытия
#define AS_MODE_SWITCH_MAX 2

// ======== Символы режимов

#define AS_MODE_SYMBOL_TEMP "S"
#define AS_MODE_SYMBOL_SENS_TEMP "C"
#define AS_MODE_SYMBOL_HSTR "H"
#define AS_MODE_SYMBOL_ERROR "E"
#define AS_MODE_SYMBOL_MESSAGE "I"

// ======== Коды сообщений

#define AS_MESSAGE_NO 0
#define AS_MESSAGE_SAVED 1
#define AS_MESSAGE_ERROR_DISPLAY 101

// ======== NTC (термистор)

// Описание с сайта AlexGyver:
// https://kit.alexgyver.ru/tutorials/thermistor
//
// - AS_NTC_RESISTOR - сопротивление балластного резистора, Ом
// - AS_NTC_BETA_COEF - бета-коэффициент термистора (см. даташит) [число в
// районе 1000-5000]
// - AS_NTC_BASE_TEMP - базовая температура термистора, градусов Цельсия (см.
// даташит) [обычно 25 градусов]
// - AS_NTC_BASE_RESISTANCE - сопротивление термистора при базовой температуре,
// Ом (см. даташит)
// - AS_NTC_RESOLUTION - разрешение АЦП, бит. По умолчанию 10

#define AS_NTC_RESISTOR 10000
#define AS_NTC_BETA_COEF 3950
#define AS_NTC_BASE_TEMP 25
#define AS_NTC_BASE_RESISTANCE 10000
#define AS_NTC_RESOLUTION 10
