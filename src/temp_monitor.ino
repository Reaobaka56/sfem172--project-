// Temperature Monitor
// Thermistor on A0, Blue LED on D8, Red LED on D9, I2C LCD on A4/A5

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// If the LCD shows nothing or garbage, try changing 0x27 to 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int thermistorPin = A0;
const int bluePin = 8;
const int redPin = 9;

// Thermistor math constants (typical 10k NTC thermistor values)
const float SERIES_RESISTOR = 10000.0;   // the 10k resistor in your divider
const float NOMINAL_RESISTANCE = 10000.0; // thermistor resistance at 25C
const float NOMINAL_TEMPERATURE = 25.0;
const float B_COEFFICIENT = 3950.0;       // common default, check your thermistor's datasheet

const float WARNING_TEMP = 30.0; // change this to whatever "high" means for you

void setup() {
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
}

void loop() {
  float tempC = readTemperature();

  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.println(" C");

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempC, 1);
  lcd.print((char)223); // degree symbol
  lcd.print("C   ");     // extra spaces to clear leftover characters

  lcd.setCursor(0, 1);

  if (tempC >= WARNING_TEMP) {
    digitalWrite(redPin, HIGH);
    digitalWrite(bluePin, LOW);
    lcd.print("Status: WARNING!");
  } else {
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, HIGH);
    lcd.print("Status: NORMAL  ");
  }

  delay(1000);
}

float readTemperature() {
  int rawADC = analogRead(thermistorPin);

  // Convert ADC reading to resistance of the thermistor
  float resistance = SERIES_RESISTOR / ((1023.0 / rawADC) - 1.0);

  // Steinhart-Hart simplified (Beta) equation
  float steinhart;
  steinhart = resistance / NOMINAL_RESISTANCE;      // (R/Ro)
  steinhart = log(steinhart);                       // ln(R/Ro)
  steinhart /= B_COEFFICIENT;                        // 1/B * ln(R/Ro)
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);  // + (1/To)
  steinhart = 1.0 / steinhart;                        // invert
  steinhart -= 273.15;                                // convert to C

  return steinhart;
}
