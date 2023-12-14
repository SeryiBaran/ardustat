# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog],
and this project adheres to [Semantic Versioning].

## [1.1.1] 2023-12-10

### Added

- Added prefix `AS_` (Ardustat => ArduStat => AS) for all `#define`

### Removed

- Removed useless settings in `config.h`
- Removed debug messages
- Removed the `F()` macro - useless for strings a couple of characters
- Removed unnecessary mode check when re-rendering the display after reading the temperature

### Changed

- Renamed some variables to reduce code. Example - `readed_temp` => `sens_temp`, `hysteresis` => `hstr`
- Merged functions `print_mode` and `put_num_at_end`
- Simplified method for checking message type (message or error)
- Simplified encoder handling function

## [1.1.0] 2023-12-10

### Added

- Added `#pragma once` in all headers
- Added [macro `F()`](https://alexgyver.ru/lessons/code-optimisation/) in all static strings

### Removed

- Removed endless error display

### Changed

- Merged Error and Message modes
- Mode symbols are included in the config
- Includes are placed in main.h
- Increased the number of comments to the code

## [1.0.1] 2023-12-09

- Переход на PlatformIO

## [1.0.0] - 2023-12-09

- First version
