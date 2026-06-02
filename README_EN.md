This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/GyverINA.svg?color=brightgreen)](https://github.com/GyverLibs/GyverINA/releases/latest/download/GyverINA.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/GyverINA.svg)](https://registry.platformio.org/libraries/gyverlibs/GyverINA)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/GyverINA?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# GyverINA
Lightweight library for INA219 and INA226 power monitor modules

### Compatibility
Compatible with all Arduino platforms (Arduino features are used)

## Contents
- [Installation](#install)
- [Initialization](#init)
- [Use of use](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found under the name **GyverINA** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/GyverINA/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="init"></a>
## Initialization
### INA219
```cpp
INA219 ina219 (Сопротивление шунта, Максимальный ожидаемый ток, Адрес на шине I2c)

INA219 ina219;                  // Standard values for INA219 module (0.1 Ohms, 3.2A, address 0x40) - suitable for one module
INA219 ina219(0x41);            // Shunt and max. Current by default, address 0x41 - suitable for several modules
INA219 ina219(0.05);            // Shunt 0.05 Ohms, max. current and default address (3.2A, 0x40) - Extended module or bare m/s
INA219 ina219(0.05, 2.0);       // Shunt 0.05 Ohm, max. expected current 2A, default address (0x40) - Added module or bare m/s
INA219 ina219(0.05, 2.0, 0x41); // Shunt 0.05 Ohm, max. expected current 2A, address 0x41 - Finished modules or bare m/s
```
### INA226
```cpp
INA226 ina226 (Сопротивление шунта, Максимальный ожидаемый ток, Адрес на шине I2c)

INA226 ina226;                  // Standard values for INA226 module (0.1 Ohms, 0.8 A, address 0x40) - suitable for one module
INA226 ina226(0x41);            // Shunt and max. Current by default, address 0x41 - suitable for several modules
INA226 ina226(0.05);            // Shunt 0.05 Ohms, max. current and default address (0.8A, 0x40) - Finished module or bare m/s
INA226 ina226(0.05, 1.5);       // Shunt 0.05 Ohm, max. expected current 1.5 A, default address (0x40) - Added module or bare m/s
INA226 ina226(0.05, 1.5, 0x41); // Shunt 0.05 Ohm, max. expected current 1.5 A, address 0x41 - Finished modules or bare m/s
```

<a id="usage"></a>
## Use of use
### INA219
```cpp
bool begin();                               // Module initialization and presence check, return false if INA219 is not found
void sleep(true / false);                   // Turning on and off low power mode, depending on the argument

void setResolution(channel, mode);          // Set resolution and averaging mode for voltage and current measurement
// channel - measurement channel
INA219_VBUS             // ADC channel measuring tire voltage (0-26v)
INA219_VSHUNT           // ADC channel measuring voltage on a shunt

// mode - mode of operation and permission
INA219_RES_9BIT         // 9 Bit - 84mx
INA219_RES_10BIT        // 10 Bit - 148mx
INA219_RES_11BIT        // 11 Bit - 276mx
INA219_RES_12BIT        // 12 Bit - 532mx
INA219_RES_12BIT_X2     // 12 bits, average of 2x - 1.06 ms
INA219_RES_12BIT_X4     // 12 Bit, average of 4x - 2.13 ms
INA219_RES_12BIT_X8     // 12 bits, average of 8x - 4.26 ms
INA219_RES_12BIT_X16    // 12 bits, average of 16x - 8.51 ms
INA219_RES_12BIT_X32    // 12 bits, average of 32x - 17.02 ms
INA219_RES_12BIT_X64    // 12 bits, average of 64x - 34.05 ms
INA219_RES_12BIT_X128   // 12 bits, average of 128x - 68.10ms

float getShuntVoltage();                    // Read the voltage on the shunt, B
float getVoltage();                         // Read the tension, In
float getCurrent();                         // Read the current, A
float getPower();                           // Read power, watts.

uint16_t getCalibration();                  // Read the calibration value (calculated automatically after start)
void setCalibration(calibration value);     // Record the calibration value (can be stored in EEPROM)
void adjCalibration(calibration offset);    // Twisting the calibration value to the specified value (can be changed on the move)
```
### INA226
```cpp
bool begin();                               // Module initialization and presence check, return false if INA226 is not found
void sleep(true / false);                   // Turning on and off low power mode depending on the argument

void setAveraging(avg);                     // Setting the number of measurement averagings (see table below)
INA226_AVG_X1
INA226_AVG_X4
INA226_AVG_X16
INA226_AVG_X64
INA226_AVG_X128
INA226_AVG_X256
INA226_AVG_X512
INA226_AVG_X1024

void setSampleTime(channel, time);          // Setting voltage and current sampling time (INA226 VBUS / INA226 VSHUNT), by default INA226 CONV 1100US
// channel - measurement channel
INA226_VBUS         // tire voltage (0-36v)
INA226_VSHUNT       // shunt

// time - sampling time (accumulation of a signal for digitization)
INA226_CONV_140US   // 140 μs
INA226_CONV_204US   // 204 μs
INA226_CONV_332US   // 332 μs
INA226_CONV_588US   // 588 μs
INA226_CONV_1100US  // 1100 μs
INA226_CONV_2116US  // 2116 μs
INA226_CONV_4156US  // 4156 μs
INA226_CONV_8244US  // 8244 μs
    
float getShuntVoltage();                    // Read the voltage on the shunt, B
float getVoltage();                         // Read the voltage, B
float getCurrent();                         // Read the current, A
float getPower();                           // Read power, watts.

uint16_t getCalibration();                  // Read the calibration value (calculated automatically after start)
void setCalibration(calibration value);     // Record the calibration value (can be stored in EEPROM)
void adjCalibration(calibration offset);    // Twisting the calibration value to the specified value (can be changed on the move)
```

<a id="example"></a>
## Example
For more examples see **examples**!

<details>
<summary>INA219</summary>

```cpp
#include <GyverINA.h>

// Create object: INA219 ina (shunt resistance, Max. expected current, I2c address);
// INA219 ina(0x41); // Standard settings for the module, but changed address
// INA219 ina(0.05f); // Standard address and max. current, but another shunt (0.05 ohms)
// INA219 ina(0.05f, 2.0f); // Standard address but different bypass (0.05 ohms) and max expected current (2A)
// INA219 ina(0.05f, 2.0f, 0x41); // Fully customizable, manual setting
INA219 ina;                       // Standard set of parameters for the Arduino module (0.1, 3.2, 0x40)

void setup() {
  // Opening the serial port
  Serial.begin(9600);
  Serial.print(F("INA219..."));

  // Check for and initialize INA219
  if (ina.begin()) {
    Serial.println(F("connected!"));
  } else {
    Serial.println(F("not found!"));
    while (1);
  }

  // You can rotate to sleep mode by calling .sleep with the true argument to wake up - call again with the indication false
  // ina.sleep(true); // Euthanize INA219
  // ina.sleep(false); // Wake up INA219

  // INA219 has the ability to calibrate current measurement using a special calibration value.
  // After launch, the library will automatically calculate and record the calibration value based on the entered data.
  // The resulting value can be read using the .getCalibration() method; to modify and/or save the EEPROM
  Serial.print(F("Calibration value: ")); Serial.println(ina.getCalibration());
  // Next, the resulting value can be changed to adjust to the real resistance of the shunt and stored in EEPROM.
  // To write a calibration value in INA219, there is a .setCalibration (value) method.
  // ina.setCalibration(ina.getCalibration() + 10); // Read-modify-write calibration value
  // You can also use the .adjCalibration(offset) method to adjust the calibration without reading directly.
  // ina.adjCalibration(10); // Increase calibration value by 10
  // ina.adjCalibration(-20); // Reduce the calibration value by 20
  // It is possible to store in EEPROM and load in INA219 precisely the calibration offset instead of the immediate value.

  // It is also possible to select the resolution of the ADC (9-12 bits) and include the built-in measurement averaging.
  // The choice of settings for measuring voltage and current are divided and determined by the constants INA219 VBUS or INA219 VSHUNT.
  // Averaging increases measurement time by reducing measurement noise, available only for 12-bit mode
  // ina.setResolution(INA219 VBUS, INA219 RES 10BIT); // Measure voltage in 10 bit mode, 12 bits by default
  // Reduced resolution speeds up measurements (see table in INA219.h) but NOT recommended
  // The use of built-in averaging is highly recommended to improve the stability of readings on noisy load
  ina.setResolution(INA219_VBUS, INA219_RES_12BIT_X4);      // Voltage in 12-bit mode + 4x averaging
  ina.setResolution(INA219_VSHUNT, INA219_RES_12BIT_X128);  // Current in 12-bit mode + 128x averaging
  
  Serial.println("");
}

void loop() {
  // Reading the tension
  Serial.print(F("Voltage: "));
  Serial.print(ina.getVoltage(), 3);
  Serial.println(F(" V"));

  // Read the current.
  Serial.print(F("Current: "));
  Serial.print(ina.getCurrent(), 3);
  Serial.println(F(" A"));

  // Reading power.
  Serial.print(F("Power: "));
  Serial.print(ina.getPower(), 3);
  Serial.println(F(" W"));

  // Reading voltage on a shunt
  Serial.print(F("Shunt voltage: "));
  Serial.print(ina.getShuntVoltage(), 6);
  Serial.println(F(" V"));

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
   Внимание!!! Пределы измерения напряжения шунта у INA226 = +/- 81.92 мВ
   При использовании модуля INA226 с шунтом 0.1 Ом макс. измеряемый ток будет I ~ 820 мА
   При использовании другого шунта, рекомендуется рассчитать его так, чтобы падение напряжения не превышало 82мВ!

   Пример:
   Макс. ожидаемый ток = 5 A
   Предел падения напряжения на шунте = 80 мВ
   R шунта = 0.08 В / 5 А = 0,016 Ом
   Шунт должен иметь сопротивление 0.016 Ом (16 мОм)
*/

// Create object: INA226 ina (shunt resistance, Max. expected current, I2c address);
// INA226 ina(0x41); // Standard settings for the module, but changed address
// INA226 ina(0.05f); // Standard address and max. current, but another shunt (0.05 ohms)
// INA226 ina(0.05f, 1.5f); // Standard address but different bypass (0.05 ohms) and max expected current (1.5 A)
// INA226 ina(0.05f, 1.5f, 0x41); // Fully customizable, manual setting
INA226 ina;                       // Standard set of parameters for the Arduino module (0.1, 0.8, 0x40)

void setup() {
  // Opening the serial port
  Serial.begin(9600);
  Serial.print(F("INA226..."));

  // Check for and initialize INA226
  if (ina.begin()) {
    Serial.println(F("connected!"));
  } else {
    Serial.println(F("not found!"));
    while (1);
  }

  // You can rotate to sleep mode by calling .sleep with the true argument to wake up - call again with the indication false
  // ina.sleep(true); // Euthanize INA226
  // ina.sleep(false); // Wake up INA226

  // INA226 has the ability to calibrate current measurement using a special calibration value
  // After launch, the library will automatically calculate and record the calibration value based on the entered data.
  // The resulting value can be read using the .getCalibration() method; to modify and/or save the EEPROM
  Serial.print(F("Calibration value: ")); Serial.println(ina.getCalibration());
  // Next, the resulting value can be changed to adjust to the real resistance of the shunt and stored in EEPROM.
  // To write a calibration value in INA226, there is a .setCalibration (value) method.
  // ina.setCalibration(ina.getCalibration() + 10); // Read-modify-write calibration value
  // You can also use the .adjCalibration(offset) method to adjust the calibration without reading directly.
  // ina.adjCalibration(10); // Increase calibration value by 10
  // ina.adjCalibration(-20); // Reduce the calibration value by 20
  // It is possible to store in EEPROM and load in INA226 precisely the calibration offset instead of the immediate value.

  // To improve noise immunity, INA226 has the ability to adjust voltage and current sampling time.
  // INA226 will capture a “piece” of a signal of a selected duration, which will improve accuracy on a noisy signal.
  // By default, the sample takes 1100 μs, but can be changed by the .setSampleTime method (channel, time).
  // For sample time options, see the table (INA226.h)
  ina.setSampleTime(INA226_VBUS, INA226_CONV_2116US);   // Increase voltage sampling time by half
  ina.setSampleTime(INA226_VSHUNT, INA226_CONV_8244US); // Increase current sampling time by 8 times

  // It is also possible to use built-in sample averaging.
  // Averaging is applied to both voltage and current and proportionally increases the digitization time.
  // Recommended on noisy load, set by .setAveraging (number of averaging) (see table in INA226.h)
  ina.setAveraging(INA226_AVG_X4); // Include built-in 4x averaging, there is no default averaging

  Serial.println("");
}

void loop() {
  // Reading the tension
  Serial.print(F("Voltage: "));
  Serial.print(ina.getVoltage(), 3);
  Serial.println(F(" V"));

  // Read the current.
  Serial.print(F("Current: "));
  Serial.print(ina.getCurrent(), 3);
  Serial.println(F(" A"));

  // Reading power.
  Serial.print(F("Power: "));
  Serial.print(ina.getPower(), 3);
  Serial.println(F(" W"));

  // Reading voltage on a shunt
  Serial.print(F("Shunt voltage: "));
  Serial.print(ina.getShuntVoltage(), 6);
  Serial.println(F(" V"));

  Serial.println("");
  delay(1000);
}

```
</details>

<a id="versions"></a>
## Versions
- v1.0
- v1.1 - fixed warnings, brushed tabulation (AlexGyver)
- v1.2 - corrected error in the calculation of ina226
- v1.2.1 - Minor examples corrections, added the ability to specify I2C pins for ESP8266/ESP32 in start methods for INA226 and INA219

<a id="feedback"></a>
## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
