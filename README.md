# PinController Web (ESP32)

Simple ESP32 project using the Arduino framework that serves a web interface to control GPIO pins.

## Tech stack
- Board: ESP32 (esp32dev)
- Framework: Arduino
- Build system: PlatformIO
- Language: C++

See [platformio.ini](platformio.ini) for the active environment:
- `platform = espressif32`
- `board = esp32dev`
- `framework = arduino`

## Requirements
- ESP32 DevKit (or compatible)
- VS Code + PlatformIO extension, or PlatformIO Core (CLI)
- USB cable for flashing

## Quick start
1. Clone this repository.
2. Open the folder in VS Code with PlatformIO, or use the CLI.
3. Configure your Wi‑Fi credentials and desired pin mappings in the source (see files under `src/`, typically `main.cpp`).
4. Build and upload:
   ```bash
   pio run
   pio run --target upload
   ```
5. Open the serial monitor to find the device IP:
   ```bash
   pio device monitor
   ```
6. Visit the device IP in your browser to use the web interface.

## Project structure
- `src/` – application source code (C++)
- `platformio.ini` – PlatformIO environment and board configuration
- `LICENSE` – project license
- `.gitignore` – ignored files

## Notes
- Make sure the selected pins are safe to drive and not used for bootstrapping.
- If you change the board or environment, update `platformio.ini` accordingly.

## License
This project is licensed under the terms described in [LICENSE](LICENSE).
