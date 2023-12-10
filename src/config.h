#pragma once

// Раскомментируй для отладки по UART
// #define ENABLE_DEBUG

// Позаимствовано у AlexGyver (https://alexgyver.ru)
#ifdef ENABLE_DEBUG
#define DEBUG(x) Serial.print(x)
#define DEBUGLN(x) Serial.println(x)
#else
#define DEBUG(x)
#define DEBUGLN(x)
#endif

// Пины
#define AS_PIN_NTC A0        // Пин термистора
#define AS_PIN_RELAY 2       // Пин реле
#define AS_PIN_ENC_A 5       // Пин энкодера A
#define AS_PIN_ENC_B 4       // Пин энкодера B
#define AS_PIN_ENC_BUTT 3    // Пин кнопки энкодера
#define AS_PIN_TM1637_CLK 11 // Пин CLK дисплея TM1637
#define AS_PIN_TM1637_DIO 12 // Пин DIO дисплея TM1637

// Настройки
#define AS_MIN_TEMP 20                 // Минимальная задаваемая температура
#define AS_MAX_TEMP 60                 // Максимальная задаваемая температура
#define AS_DEFAULT_TEMP 20             // Заданная температура по умолчанию
#define AS_DEFAULT_SENS_TEMP 99        // Температура с датчика по умолчанию
#define AS_MIN_HSTR 1                  // Минимальный гистерезис
#define AS_MAX_HSTR 10                 // Максимальный гистерезис
#define AS_DEFAULT_HSTR 2              // Гистерезис по умолчанию
#define AS_TEMP_READ_TIME 500          // Время между замерами температуры
#define AS_RELAY_UPDATE_TIME 3000      // Минимальное время между изменением состояния реле
#define AS_MESSAGE_TIME 3000           // Время показа сообщения
#define AS_SETTINGS_SAVE_TIME 1000 * 5 // Время бездействия, после которого сохраняются настройки. 1000 * 5 - через 5 секунд после последнего изменения
#define AS_TM1637_DIGITS_NUM 4         // Количество разрядов на дисплее

// Настройки EEPROM
#define AS_EEPROM_INIT_MARKER_VALUE 10 // Флаг по которому устройство будет понимать что уже включалось и в нем что-то сохранено (взято от балды)
#define AS_EEPROM_SETTINGS_ADDR 0      // Начальный адрес в EEPROM с настройками

// Режимы дисплея
#define AS_MODE_TEMP 0
#define AS_MODE_SENS_TEMP 1
#define AS_MODE_HSTR 2
#define AS_MODE_MESSAGE 101
#define AS_MODE_SWITCH_MIN 0         // "Индекс" первого режима доступного для переключения
#define AS_MODE_SWITCH_MAX 2         // "Индекс" последнего
#define AS_MODE_DEFAULT AS_MODE_TEMP // Стандартный режим

// Символы режимов
#define AS_MODE_SYMBOL_TEMP "S"
#define AS_MODE_SYMBOL_SENS_TEMP "C"
#define AS_MODE_SYMBOL_HSTR "H"
#define AS_MODE_SYMBOL_ERROR "E"
#define AS_MODE_SYMBOL_MESSAGE "I"

// Коды сообщений
#define AS_MESSAGE_NO 0
#define AS_MESSAGE_SAVED 1
#define AS_MESSAGE_ERROR_DISPLAY 101
#define AS_MESSAGE_FIRST 1
#define AS_MESSAGE_LAST 99
#define AS_MESSAGE_ERROR_FIRST 100
#define AS_MESSAGE_ERROR_LAST 199

// NTC (термистор) (https://kit.alexgyver.ru/tutorials/thermistor/)
#define AS_NTC_RESISTOR 10000        // Сопротивление резистора подключенного с NTC для создания делителя напряжения
#define AS_NTC_BETA_COEF 3950        // Бета-коэффициент NTC
#define AS_NTC_BASE_TEMP 25          // Температура при которой у NTC сопротивление NTC_BASE_RESISTANCE
#define AS_NTC_BASE_RESISTANCE 10000 // Базовое сопротивление NTC
#define AS_NTC_RESOLUTION 10         // Разрешение NTC
