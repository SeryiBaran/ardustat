# Ardustat

Схема в Fritzing:

![Fritzing](./readme_assets/fritzing.png)

Термостат на Arduino Nano + NTC с 4-разрядным 7-сегментным дисплеем.

Код и схема создавались в онлайн-симуляторе [Wokwi](https://wokwi.com/) и не проверялись в реальности.

Настройки сохраняются в EEPROM. В Wokwi нет внешнего EEPROM, поэтому используется встроенный в микроконтроллер - но лучше заменить на внешний.

## Зависимости (в папке libs)

- [EncButton](https://github.com/GyverLibs/EncButton)
- [GyverNTC](https://github.com/GyverLibs/GyverNTC)
- [GyverSegment](https://github.com/GyverLibs/GyverSegment)
- [PinChangeInterrupt](https://github.com/NicoHood/PinChangeInterrupt)
- [TimerMs](https://github.com/GyverLibs/TimerMs)

## [Руководство пользователя - в виде .Docx документа (для печати)](./USER_MANUAL.docx)

## [А как прошить, а как собрать](https://alexgyver.ru/arduino-first/)
