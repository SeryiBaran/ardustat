# Ardustat

Схема в Fritzing:

![Fritzing](./readme_assets/fritzing.png)

Термостат на Arduino Nano + NTC с 4-разрядным 7-сегментным дисплеем.

Код и схема создавались в онлайн-симуляторе [Wokwi](https://wokwi.com/) и не проверялись в реальности.

Настройки сохраняются в EEPROM. В Wokwi нет внешнего EEPROM, поэтому используется встроенный в микроконтроллер - но лучше заменить на внешний.

Занятые ресурсы МК по мнению Platformio:

```
RAM:   [=         ]  10.6% (used 218 bytes from 2048 bytes)
Flash: [===       ]  28.7% (used 8814 bytes from 30720 bytes)
```

## Зависимости (устанавливаются через Platformio)

- EncButton
- GyverNTC
- GyverSegment
  - GyverIO
- PinChangeInterrupt
- TimerMs

## Электронные компоненты

- Arduino Nano
- Реле на 5V
- Энкодер с кнопкой
- Термистор
- Резистор, лучше равный сопротивлению термистора
- Дисплей TM1637

## [Руководство пользователя - в виде .Docx документа (для печати)](./USER_MANUAL.docx)

## [А как прошить, а как собрать](https://alexgyver.ru/arduino-first/)
