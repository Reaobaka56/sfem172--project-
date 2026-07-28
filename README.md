# Arduino Temperature Monitor

A simple temperature monitoring and warning system built with an Arduino Uno, an NTC thermistor, two LEDs, and a 16x2 I2C LCD.

- **Blue LED** — lights up when the temperature is in the normal range
- **Red LED** — lights up when the temperature crosses a warning threshold
- **LCD** — shows the live temperature reading and current status text

## Hardware

| Component | Qty |
|---|---|
| Arduino Uno | 1 |
| NTC thermistor (10k) | 1 |
| 10kΩ resistor | 1 |
| 220Ω resistor | 2 |
| Blue LED | 1 |
| Red LED | 1 |
| 16x2 LCD with I2C backpack | 1 |
| Breadboard + jumper wires | as needed |

## Wiring

See [`docs/wiring_diagram.svg`](docs/wiring_diagram.svg) for the full schematic.

**Thermistor (voltage divider):**
- Thermistor leg 1 → Arduino 5V
- Thermistor leg 2 → Arduino A0, and → 10kΩ resistor → GND

**Blue LED (normal indicator):**
- Long leg → 220Ω resistor → Arduino D8
- Short leg → GND

**Red LED (warning indicator):**
- Long leg → 220Ω resistor → Arduino D9
- Short leg → GND

**LCD (I2C):**
| LCD pin | Arduino pin |
|---|---|
| GND | GND |
| VCC | 5V |
| SDA | A4 |
| SCL | A5 |

All grounds share a single ground rail on the breadboard, which connects back to one Arduino GND pin.

## Software

The sketch is in [`src/temp_monitor.ino`](src/temp_monitor.ino).

### Requirements

- [Arduino IDE](https://www.arduino.cc/en/software)
- `LiquidCrystal_I2C` library (Sketch → Include Library → Manage Libraries → search "LiquidCrystal I2C")

### How it works

1. Reads the analogue voltage at A0 from the thermistor voltage divider.
2. Converts the ADC reading to resistance, then to temperature (°C) using the simplified Steinhart–Hart (Beta) equation.
3. If temperature ≥ `WARNING_TEMP` (default 30°C): red LED on, blue LED off, LCD shows "WARNING!".
4. Otherwise: blue LED on, red LED off, LCD shows "NORMAL".

### Configuration

Edit these constants at the top of `temp_monitor.ino` to match your hardware:

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);  // try 0x3F if the LCD shows nothing
const float B_COEFFICIENT = 3950.0;   // check your thermistor's datasheet
const float WARNING_TEMP = 30.0;      // your warning threshold, in °C
```

## Docs

- [`docs/wiring_diagram.svg`](docs/wiring_diagram.svg) — full breadboard wiring schematic
- [`docs/research_proposal.docx`](docs/research_proposal.docx) — mini research proposal linking this project to circuit theory topics (ELYM 127 — Introduction to Electronics)

## License

MIT — see [LICENSE](LICENSE).
