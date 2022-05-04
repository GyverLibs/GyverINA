This is an automatic translation, may be incorrect in some places. See sources and examples!

# GyverINA
Lightweight library for INA219 and INA226 power-monitor modules

### Compatibility
Compatible with all Arduino platforms (using Arduino functions)

## Content
- [Install](#install)
- [Initialization](#init)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found under the name **GyverINA** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download library](https://github.com/GyverLibs/GyverINA/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Initialization
### INA219
```cpp
INA219 ina219 (Shunt resistance, Maximum prospective current, Address on I2c bus)

INA219 ina219; // Standard valuesCranberries for the INA219 module (0.1 Ohm, 3.2A, address 0x40) - suitable for one module
INA219 ina219(0x41); // Shunt and max. default current, address 0x41 - suitable for several modules
INA219 ina219(0.05); // Shunt 0.05 ohm, max. default current and address (3.2A, 0x40) - Doped module or bare m/s
INA219 ina219(0.05, 2.0); // Shunt 0.05 Ohm, max. expected current 2A, default address (0x40) - Doped module or bare m/s
INA219 ina219(0.05, 2.0, 0x41); // Shunt 0.05 Ohm, max. expected current 2A, address 0x41 - Doped modules or bare m/s
```
### INA226
```cpp
INA226 ina226 (Shunt resistance, Maximum prospective current, I2c bus address)

INA226 ina226; // Standard values ​​for the INA226 module (0.1 ohm, 0.8 A, address 0x40) - suitable for one module
INA226 ina226(0x41); // Shunt and max. default current, address 0x41 - suitable for several modules
INA226 ina226(0.05); // Shunt 0.05 ohm, max. default current and address (0.8 A, 0x40) - Finished module or bare m/s
INA226 ina226(0.05, 1.5); // Shunt 0.05 Ohm, max. expected current 1.5 A, default address (0x40) - Finished module or bare m/s
INA226 ina226(0.05, 1.5, 0x41); // Shunt 0.05 Ohm, max. expected current 1.5 A, address 0x41 - Doped modules or bare m/s
```

<a id="usage"></a>
## Usage
### INA219
```cpp
boolbegin(); // Initialize the module and check for presence, will return false if INA219 is not found
void sleep(true / false); // Enable or disable low power mode, depending on the argument

void setResolution(channel, mode); // Set resolution and averaging mode for voltage and current measurements
// channel - measurement channel
INA219_VBUS // ADC channel that measures bus voltage (0-26V)
INA219_VSHUNT // ADC channel that measures shunt voltage

// mode - operation mode and resolution
INA219_RES_9BIT // 9 Bit - 84µs
INA219_RES_10BIT // 10 Bit - 148µs
INA219_RES_11BIT // 11 Bit - 276µs
INA219_RES_12BIT // 12 Bit - 532µs
INA219_RES_12BIT_X2 // 12 bits, average of 2x - 1.06ms
INA219_RES_12BIT_X4 // 12 bits, average of 4x - 2.13ms
INA219_RES_12BIT_X8 // 12 bits, average of 8x - 4.26ms
INA219_RES_12BIT_X16 // 12 bits, average of 16x - 8.51ms
INA219_RES_12BIT_X32 // 12 bits, average of 32x - 17.02ms
INA219_RES_12BIT_X64 // 12 bits, average of 64x - 34.05ms
INA219_RES_12BIT_X128 // 12 bits, average of 128x - 68.10ms

float getShuntVoltage(); // Read shunt voltage
float getVoltage(); // Read voltage
float getCurrent(); // Read current
float getPower(); // Read power

uint16_t getCalibration(); // Read calibration value (calculated automatically after start)
void setCalibration(calibration value); // Write calibration value (you can store it in EEPROM)
void adjCalibration(calibration offset); // Twist the calibration value to the specified value (can be changed on the go)
```
### INA226
```cpp
boolbegin(); // Initialize the module and check for presence, will return false if INA226 is not found
void sleep(true / false); // Enable or disable low power mode depending on the argument

void setAveraging(avg); // Set the number of measurement averages (see table below)
INA226_AVG_X1
INA226_AVG_X4
INA226_AVG_X16
INA226_AVG_X64
INA226_AVG_X128
INA226_AVG_X256
INA226_AVG_X512
INA226_AVG_X1024

void setSampleTime(channel, time); // Set voltage and current sampling time (INA226_VBUS / INA226_VSHUNT), default INA226_CONV_1100US
// channel - measurement channel
INA226_VBUS // bus voltage (0-36v)
INA226_VSHUNT // shunt voltage

// time - sampling time (accumulativesignal for digitization)
INA226_CONV_140US // 140 µs
INA226_CONV_204US // 204 µs
INA226_CONV_332US // 332 µs
INA226_CONV_588US // 588 µs
INA226_CONV_1100US // 1100 µs
INA226_CONV_2116US // 2116 µs
INA226_CONV_4156US // 4156 µs
INA226_CONV_8244US // 8244 µs
    
float getShuntVoltage(); // Read shunt voltage
float getVoltage(); // Read voltage
float getCurrent(); // Read current
float getPower(); // Read power

uint16_t getCalibration(); // Read calibration value (calculated automatically after start)
void setCalibration(calibration value); // Write calibration value (you can store it in EEPROM)
void adjCalibration(calibration offset); // Twist the calibration value to the specified value (can be changed on the go)
```

<a id="example"></a>
## Example
See **examples** for other examples!

<details>
<summary>INA219</summary>

```cpp
#include <GyverINA.h>

// Create an object: INA219 ina(Shunt resistance, Max expected current, I2c address);
// INA219 ina(0x41); // Standard settings for the module, but changed address
// INA219 ina(0.05f); // Standard address and max. current but different shunt (0.05 ohm)
// INA219 ina(0.05f, 2.0f); // Standard address, but different shunt (0.05 ohm) and max. prospective current (2A)
// INA219 ina(0.05f, 2.0f, 0x41); // Fully customizable option, manual specification of parameters
INA219ina; // Standard set of parameters for the Arduino module (0.1, 3.2, 0x40)

void setup() {
  // Open serial port
  Serial.begin(9600);
  Serial.print(F("INA219..."));

  // Check for presence and initialize INA219
  if (ina.begin()) {
    Serial.println(F("connected!"));
  } else {
    Serial.println(F("not found!"));
    while(1);
  }

  // Can be put to sleep by calling .sleep with a true argument to wake upfishing - call again with indication false
  // ina.sleep(true); // Put INA219 to sleep
  // ina.sleep(false); // Wake up INA219

  // INA219 has built-in calibration for current measurement, using a special calibration value
  // After starting, the library will automatically calculate and write the calibration value based on the entered data
  // The resulting value can be read using the .getCalibration() method; to change and/or save to EEPROM
  Serial.print(F("Calibration value: ")); Serial.println(ina.getCalibration());
  // Further, the obtained value can be changed to adjust to the actual resistance of the shunt and stored in EEPROM
  // To write the calibration value to the INA219 there is a method .setCalibration(value);
  // ina.setCalibration(ina.getCalibration() + 10); // Read-modify-write calibration value
  // Also, you can use the .adjCalibration(offset); for calibration adjustment without direct reading
  //ina.adjCalibration(10); // Increase calibration value by 10
  // ina.adjCalibration(-20); // Decrease calibration value by 20
  // It is possible to store in EEPROM and load into INA219 exactly the calibration offset instead of a direct value

  // It is also possible to select the resolution of the ADC (9-12 bits) and enable the built-in averaging of measurements
  // Select settings for voltage and current measurement separated and defined by INA219_VBUS or INA219_VSHUNT constants
  // Averaging increases measurement time by reducing measurement noise, only available for 12-bit mode
  // ina.setResolution(INA219_VBUS, INA219_RES_10BIT); // Measure voltage in 10 bit mode, 12 bit by default
  // Using a lower resolution speeds up measurements (see table in INA219.h), but is NOT recommended
  // Use of built-in averaging is highly recommended to increase the stability of readings on a noisy load
  ina.setResolution(INA219_VBUS, INA219_RES_12BIT_X4); // Napdressing in 12-bit mode + 4x averaging
  ina.setResolution(INA219_VSHUNT, INA219_RES_12BIT_X128); // Current in 12-bit mode + 128x average
  
  Serial.println("");
}

void loop() {
  // Read voltage
  Serial.print(F("Voltage: "));
  Serial.print(ina.getVoltage(), 3);
  Serial.println(F("V"));

  // Read the current
  Serial.print(F("Current: "));
  Serial.print(ina.getCurrent(), 3);
  Serial.println(F("A"));

  // Read power
  Serial.print(F("Power: "));
  Serial.print(ina.getPower(), 3);
  Serial.println(F("W"));

  // Read the voltage on the shunt
  Serial.print(F("Shunt voltage: "));
  Serial.print(ina.getShuntVoltage(), 6);
  Serial.println(F("V"));

  Serial.println("");
  delay(1000);
}
```
</details>
<details>
<summary>INA226</summary>

```cpp
#include <GyverINA.h>

/*
   Attention!!! Shunt voltage measurement limits for INA226 = +/- 81.92 mV
   When using the INA226 module with a 0.1 Ω shunt, max. the measured current will be I~820mA
   When using a different shunt, it is recommended to size it so that the voltage drop does not exceed 82mV!

   Example:
   Max. prospective current = 5 A
   Shunt voltage drop limit = 80 mV
   R shunt = 0.08 V / 5 A = 0.016 ohm
   The shunt must have a resistance of 0.016 ohm (160 mOhm)
*/

// Create an object: INA226 ina(Shunt resistance, Max expected current, I2c address);
// INA226 ina(0x41); // Standard settings for the module, but changed address
// INA226 ina(0.05f); // Standard address and max. current but different shunt (0.05 ohm)
// INA226 ina(0.05f, 1.5f); // Standard address, but different shunt (0.05 ohm) and max. prospective current (1.5 A)
// INA226 ina(0.05f, 1.5f, 0x41); // Fully customizable option, manual specification of parameters
INA226ina; // Standard set of parameters for the Arduino module (0.1, 0.8, 0x40)

void setup() {
  // Open serial port
  Serial.begin(9600);
  Serial.print(F("INA226..."));

  // Check for existence and initializeINA226
  if (ina.begin()) {
    Serial.println(F("connected!"));
  } else {
    Serial.println(F("not found!"));
    while(1);
  }

  // Can be put to sleep by calling .sleep with an argument of true, to wake it up - call it again with false
  // ina.sleep(true); // Put INA226 to sleep
  // ina.sleep(false); // Wake up INA226

  // INA226 has the ability to calibrate the current measurement internally, using a special calibration value
  // After starting, the library will automatically calculate and write the calibration value based on the entered data
  // The resulting value can be read using the .getCalibration() method; to change and/or save to EEPROM
  Serial.print(F("Calibration value: ")); Serial.println(ina.getCalibration());
  // Further, the obtained value can be changed to adjust to the actual resistance of the shunt and stored in EEPROM
  // To write the calibration value to the INA226 there is a method .setCalibration(value);
  // ina.setCalibration(ina.getCalibration() + 10); // Read-modify-write calibration value
  // Also, you can use the .adjCalibration(offset); for calibration adjustment without direct reading
  //ina.adjCalibration(10); // Increase calibration value by 10
  // ina.adjCalibration(-20); // Decrease calibration value by 20
  // It is possible to store in EEPROM and load into INA226 exactly the calibration offset instead of a direct value

  // To improve noise immunity, the INA226 has the ability to adjust the voltage and current sampling times
  // INA226 will capture a "chunk" of the signal of the selected duration, which will improve accuracy on a noisy signal
  // By default, sampling takes 1100 µs, but can be changed using the .setSampleTime(channel, time) method;
  // See table for sampling times (FILE INA226.h)
  ina.setSampleTime(INA226_VBUS, INA226_CONV_2116US); // Double the voltage sampling time
  ina.setSampleTime(INA226_VSHUNT, INA226_CONV_8244US); // Increase the current sampling time by 8 times

  // It is also possible to use the built-in sample averaging
  // Averaging is applied to both voltage and current and proportionally increases the digitization time
  // Recommended for noisy loads, set by .setAveraging(number of averages) (see table in INA226.h)
  ina.setAveraging(INA226_AVG_X4); // Turn on the built-in 4x averaging, no averaging by default

  Serial.println("");
}

void loop() {
  // Read voltage
  Serial.print(F("Voltage: "));
  Serial.print(ina.getVoltage(), 3);
  Serial.println(F("V"));

  // Read the current
  Serial.print(F("Current: "));
  Serial.print(ina.getCurrent(), 3);
  Serial.println(F("A"));

  // Read power
  Serial.print(F("Power: "));
  Serial.print(ina.getPower(), 3);
  Serial.println(F("W"));

  // Read the voltage on the shunt
  Serial.print(F("Shunt voltage: "));
  Serial.print(ina.getShuntVoltage(), 6);
  Serial.println(F("V"));

  Serial.println("");
  delay(1000);
}

```
</details>

<a id="versions"></a>
## Versions
- v1.0
- v1.1 - fixed varnings, tweaked tabs (AlexGyver)
- v1.2 - fixed bug in ina226 calculations

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!