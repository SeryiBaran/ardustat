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
#define PIN_NTC A0        // Пин термистора
#define PIN_RELAY 2       // Пин реле
#define PIN_ENC_A 5       // Пин энкодера A
#define PIN_ENC_B 4       // Пин энкодера B
#define PIN_ENC_BUTT 3    // Пин кнопки энкодера
#define PIN_TM1637_CLK 11 // Пин CLK дисплея TM1637
#define PIN_TM1637_DIO 12 // Пин DIO дисплея TM1637

// Настройки
#define MIN_SETTED_TEMP 20          // Минимальная задаваемая температура
#define MAX_SETTED_TEMP 60          // Максимальная задаваемая температура
#define DEFAULT_SETTED_TEMP 20      // Заданная температура по умолчанию
#define DEFAULT_READED_TEMP 99      // Температура с датчика по умолчанию
#define MIN_HYSTERESIS 1            // Минимальный гистерезис
#define MAX_HYSTERESIS 10           // Максимальный гистерезис
#define DEFAULT_HYSTERESIS 2        // Гистерезис по умолчанию
#define TEMP_SET_STEP 1             // Шаг изменения температуры
#define HYSTERESIS_SET_STEP 1       // Шаг изменения гистерезиса
#define ENCODER_CHANGE_DIR -1       // Направление изменения параметра с помощью энкодера. -1 - увеличение по часовой, 1 - увеличение против часовой
#define TEMP_READ_TIME 500          // Время между замерами температуры
#define RELAY_UPDATE_TIME 3000      // Минимальное время между изменением состояния реле
#define MESSAGE_TIME 3000           // Время показа сообщения
#define SETTINGS_SAVE_TIME 1000 * 5 // Время бездействия, после которого сохраняются настройки. 1000 * 5 - через 5 секунд после последнего изменения
#define TM1637_DIGITS_NUM 4         // Количество разрядов на дисплее

// Настройки EEPROM
#define EEPROM_INIT_MARKER_VALUE 10 // Флаг по которому устройство будет понимать что уже включалось и в нем что-то сохранено (взято от балды)
#define EEPROM_SETTINGS_ADDR 0      // Начальный адрес в EEPROM с настройками

// Режимы дисплея
#define MODE_SETTED_TEMP 0
#define MODE_READED_TEMP 1
#define MODE_HYSTERESIS 2
#define MODE_ERROR 100
#define MODE_MESSAGE 101
#define MODE_SWITCH_MIN 0             // "Индекс" первого режима доступного для переключения
#define MODE_SWITCH_MAX 2             // "Индекс" последнего
#define MODE_DEFAULT MODE_SETTED_TEMP // Стандартный режим

// Коды ошибок
#define NO_ERROR 0
#define DISPLAY_ERROR 1

// Коды сообщений
#define MESSAGE_SAVED 1

// NTC (термистор) (https://kit.alexgyver.ru/tutorials/thermistor/)
#define NTC_RESISTOR 10000        // Сопротивление резистора подключенного с NTC для создания делителя напряжения
#define NTC_BETA_COEF 3950        // Бета-коэффициент NTC
#define NTC_BASE_TEMP 25          // Температура при которой у NTC сопротивление NTC_BASE_RESISTANCE
#define NTC_BASE_RESISTANCE 10000 // Базовое сопротивление NTC
#define NTC_RESOLUTION 10         // Разрешение NTC
