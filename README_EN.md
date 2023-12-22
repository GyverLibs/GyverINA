This is an automatic translation, may be incorrect in some places. See sources and examples!

# Gyverina
Light library for Power-monitor modules Ina219 and Ina226

## compatibility
Compatible with all arduino platforms (used arduino functions)

## Content
- [installation] (# Install)
- [initialization] (#init)
- [use] (#usage)
- [Example] (# Example)
- [versions] (#varsions)
- [bugs and feedback] (#fedback)

<a id="install"> </a>
## Installation
- The library can be found by the name ** gyverina ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/gyverina/archive/refs/heads/main.zip). Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!


<a id="init"> </a>
## initialization
### ina219
`` `CPP
Ina219 ina219 (shunt resistance, maximum expected current, address on the I2C tire)

Ina219 ina219;// Standard values for the ina219 module (0.1 Ohm, 3.2a, address 0x40) - suitable for one module
Ina219 ina219 (0x41);// Shunt and Max.default current, address 0x41 is suitable for several modules
Ina219 ina219 (0.05);// Shunt 0.05 Ohm, max.current and default address (3.2a, 0x40) - a doped module or naked m/s
Ina219 ina219 (0.05, 2.0);// Shunt 0.05 Ohm, max.Expected current 2A, default address (0x40) - a doped module or naked m/s
Ina219 ina219 (0.05, 2.0, 0x41);// Shunt 0.05 Ohm, max.Expected current 2A, address 0x41 - Dopilized modules or naked m/s
`` `
### ina226
`` `CPP
Ina226 Ina226 (shunt resistance, maximum expected current, address on the I2C tire)

Ina226 ina226;// Standard values for the ina226 module (0.1 Ohm, 0.8 a, address 0x40) - suitable for one module
Ina226 ina226 (0x41);// Shunt and Max.default current, address 0x41 is suitable for several modules
Ina226 ina226 (0.05);// Shunt 0.05 Ohm, max.Current and default address (0.8 A, 0x40) - a doped module or naked m/s
Ina226 ina226 (0.05, 1.5);// Shunt 0.05 Ohm, max.Expected current 1.5 A, default address (0x40) is a doped module or naked m/s
Ina226 ina226 (0.05, 1.5, 0x41);// Shunt 0.05 Ohm, max.Expected current 1.5 a, address 0x41 - Doped modules or naked m/s
`` `

<a id="usage"> </a>
## use
### ina219
`` `CPP
Bool Begin ();// Initialization of the module and checking the presence, will return FALSE if INA219 is not found
VOID Sleep (True / False);// turning on and off the low energy consumption mode, depending on the argument

VOID Setresolution (Channel, Mode);// Installation of permission and averaging mode for measuring voltage and current
// Channel - measurement channel
Ina219_vbus // Channel ADC, measuring the tire voltage (0-26V)
Ina219_vshunt // Channel ADC, which measures the voltage in shunt

// mode - mode of operation and resolution
Ina219_res_9bit // 9 bit - 84Mks
Ina219_res_10bit // 10 bit - 148MKS
Ina219_res_11BIT // 11 bit - 276MKS
Ina219_res_12BIT // 12 bit - 532MKS
Ina219_res_12bit_x2 // 12 bit, average of 2x - 1.06 ms
Ina219_res_12bit_x4 // 12 bits, average of 4x - 2.13 ms
Ina219_res_12bit_x8 // 12 bit, average of 8x - 4.26 ms
Ina219_res_12bit_x16 // 12 bits, average of 16x - 8.51 ms
Ina219_res_12bit_x32 // 12 bits, average of 32x - 17.02 ms
Ina219_res_12bit_x64 // 12 bits, average of 64x - 34.05 ms
Ina219_res_12bit_x128 // 12 bit, average of 128x - 68.10 ms

Float Getshuntvoltage ();// Read the voltage in shunt
Float getvoltage ();// Read the voltage
Float getcurrent ();// Read the current
Float getpower ();// Read power

uint16_t getcalibration ();// read the calibration value (after the start is calculated automatically)
Void setcalibration (Calibration Value);// write down the calibration value (you can store it in EEPROM)
Void Adjcalibration (Calibration Offseet);// twist the calibration value to the indicated value (can be changed on the go)
`` `
### ina226
`` `CPP
Bool Begin ();// Initialization of the module and checking the presence, will return FALSE if INA226 is not found
VOID Sleep (True / False);// turning on and off the low energy consumption mode depending on the argument

VOID setaveraging (AVG);// Installation of the number of measurement averages (see table below)
Ina226_avg_x1
Ina226_Avg_x4
Ina226_Avg_x16
Ina226_Avg_x64
Ina226_Avg_x128
Ina226_avg_x256
Ina226_avg_x512
Ina226_Avg_x1024

VOID setsampletime (Channel, Time);// Installation of voltage and current sampling time (Ina226_vbus / ina226_vshunt), by default INA226_CONV_1100us
// Channel - measurement channel
Ina226_vbus // tire voltage (0-36V)
Ina226_vshunt // Schunt voltage

// time - sample time (signal accumulation for digitization)
Ina226_conv_140us // 140 μs
Ina226_conv_204us // 204 μs
Ina226_conv_332us // 332 μs
Ina226_Conv_588us // 588 μs
Ina226_Conv_1100us // 1100 μs
Ina226_Conv_2116us // 2116 μs
Ina226_Conv_4156us // 4156 μs
Ina226_conv_8244us // 8244 μs
    
Float Getshuntvoltage ();// Read the voltage in shunt
Float getvoltage ();// Read the voltage
Float getcurrent ();// Read the current
Float getpower ();// Read power

uint16_t getcalibration ();// read the calibration value (after the start is calculated automatically)
Void setcalibration (Calibration Value);// write down the calibration value (you can store it in EEPROM)
Void Adjcalibration (Calibration Offseet);// twist the calibration value to the indicated value (can be changed on the go)
`` `

<a id="EXAMPLE"> </a>
## Example
The rest of the examples look at ** Examples **!

<details>
<summary> ina219 </summary>

`` `CPP
#include <gyverina.h>

// Create Object: Ina219 Ina (Shunt resistance, Max. Expected current, I2C address);
// ina219 ina (0x41);// standard settings for the module, but a modified address
// ina219 ina (0.05F);// standard address and max.current, but another shunt (0.05Cranberries om)
// ina219 ina (0.05F, 2.0F);// standard address, but another shunt (0.05 Ohms) and Max.Expected current (2A)
// ina219 ina (0.05f, 2.0f, 0x41);// Fully adjustable option, manual indication of parameters
Ina219 ina;// Standard set of parameters for Arduino module (0.1, 3.2, 0x40)

VOID setup () {
  // Open the sequential port
  Serial.Begin (9600);
  Serial.print ("ina219 ..."));

  // Check the availability and initialize ina219
  if.Begin ()) {
    Serial.println (F ("Connected!"));
  } else {
    Serial.println (f ("not found!"));
    While (1);
  }

  // you can rope into sleep mode by calling .Sleep with the True argument, to wake up, we call again with FALSE.
  // ina.sleep (true);// Esphy Ina219
  // ina.sleep (false);// Wake in Ina219

  // Ina219 has the ability to integrated calibration of current measurement, using a special calibration value
  // After starting, the library will automatically calculate and record the calibration value based on the entered data
  // the resulting value can be read using the .getcalibration () method;To change and/or conservation in EEPROM
  Serial.print ("Calibration value:"));Serial.println (ina.getcalibration ());
  // Next, the resulting value can be changed for adjustment to the real resistance of the shunt and saved in EEPROM
  // to record the calibration value in INA219, there is a method. SetCalibration (Value);
  // ina.setcalibration (ina.getcalibration () + 10);// Read-modify the calibration value
  // also, you can use the method of .adjcalibration (Offset);to adjust calibration without direct reading
  // ina.adjcalibration (10);// increase the calibration value by 10
  // ina.adjcalibration (-20);// Reduce the calibration value by 20
  // can be stored in EEPROM and uploaded to INA219 precisely the displacement of calibration instead of a direct value

  // It is also possible to choose a resolution of ADC (9-12 bits) and enable the built-in averaging measurements
  // The selection of settings for measuring voltage and current are divided and determined by the constants ina219_vbus or ina219_vshunt
  // Avoiding increases measurement time, reducing measurement noise, is available only for 12 bit mode
  // ina.setresolution (ina219_vbus, ina219_res_10bit);// Measure voltage in 10 bit mode, 12 bits by default
  // Using a reduced resolution accelerates the measurement (see table in Ina219.h), but not recommended
  // The use of built -in averaging is extremely recommended to increase the stability of indications on the noisy load
  Ina.Setresolution (Ina219_vbus, Ina219_res_12BIT_X4);// voltage in 12 bit mode + 4 -multiple averaging
  Ina.Setresolution (Ina219_vshunt, Ina219_res_12BIT_X128);// current in 12 bit mode + 128x multiple averages
  
  Serial.println ("");
}

VOID loop () {
  // read tension
  Serial.print ("voltage:"));
  Serial.print (ina.getvoltage (), 3);
  Serial.println ("v"));

  // read the current
  Serial.print ("Current:"));
  Serial.print (ina.getcurrent (), 3);
  Serial.println ("a"));

  // read power
  Serial.print ("Power:"));
  Serial.print (ina.getpower (), 3);
  Serial.println ("w"));

  // Read the voltage in shunt
  Serial.print ("shunt voltage:"));
  Serial.print (ina.getshuntvoltage (), 6);
  Serial.println ("v"));

  Serial.println ("");
  DELAY (1000);
}
`` `
</details>
<details>
<summary> ina226 </summary>

`` `CPP
#include <gyverina.h>

/*
   Attention!!!The limits of measuring the shunt voltage in INA226 = +/- 81.92 MV
   When using the INA226 module with a shunt 0.1 ohm max.The measured current will be I ~ 820 mA
   When using another shunt, it is recommended to calculate it so that the voltage drop does not exceed 82 mv!

   Example:
   Max.expected current = 5 A
   The limit of the voltage drop in shunt = 80 mV
   R shunt = 0.08 c / 5 a = 0.016 Ohms
   Shunt should have a resistance of 0.016 Ohms (16 MOM)
*/

// Create Object: Ina226 Ina (Shunt resistance, maKS.expected current, I2C address);
// ina226 ina (0x41);// standard settings for the module, but a modified address
// ina226 ina (0.05F);// standard address and max.current, but another shunt (0.05 Ohms)
// ina226 ina (0.05F, 1.5F);// standard address, but another shunt (0.05 Ohms) and Max.Expected current (1.5 a)
// ina226 ina (0.05F, 1.5F, 0x41);// Fully adjustable option, manual indication of parameters
Ina226 ina;// Standard set of parameters for arduino module (0.1, 0.8, 0x40)

VOID setup () {
  // Open the sequential port
  Serial.Begin (9600);
  Serial.print ("ina226 ..."));

  // Check the availability and initialize ina226
  if.Begin ()) {
    Serial.println (F ("Connected!"));
  } else {
    Serial.println (f ("not found!"));
    While (1);
  }

  // you can rope into sleep mode by calling .Sleep with the True argument, to wake up, we call again with FALSE.
  // ina.sleep (true);// Esphy Ina226
  // ina.sleep (false);// Wake in ina226

  // Ina226 has the ability to integrated calibration of current measurement, using a special calibration value
  // After starting, the library will automatically calculate and record the calibration value based on the entered data
  // the resulting value can be read using the .getcalibration () method;To change and/or conservation in EEPROM
  Serial.print ("Calibration value:"));Serial.println (ina.getcalibration ());
  // Next, the resulting value can be changed for adjustment to the real resistance of the shunt and saved in EEPROM
  // to record the calibration value in INA226 there is a method. SetCalibration (Value);
  // ina.setcalibration (ina.getcalibration () + 10);// Read-modify the calibration value
  // also, you can use the method of .adjcalibration (Offset);to adjust calibration without direct reading
  // ina.adjcalibration (10);// increase the calibration value by 10
  // ina.adjcalibration (-20);// Reduce the calibration value by 20
  // can be stored in EEPROM and loaded in INA226 precisely the calibration displacement instead of a direct value

  // In order to increase the nozzles of Ina226, it has the ability to configure the time of sample of voltage and current
  // Ina226 will capture the "piece" of the signal of the selected duration, which will increase the accuracy on the noisy signal
  // by default, the sample takes 1100 μs, but can be changed by the method of .Setsampletime (channel, time);
  // Sample time options see the table (file ina226.h)
  Ina.setsampletime (ina226_vbus, ina226_conv_2116us);// We will increase the voltage sample time
  Ina.setsampletime (Ina226_vshunt, Ina226_Conv_8244US);// We will increase the time of current sample by 8 times

  // It is also possible to use the built -in averaging samples
  // Availability is used for voltage and for current and proportionally increases digitization time
  // is recommended on a noisy load, set by the method of .Setaveraaging (number of overalls) (see table in INA226.H)
  Ina.Setaveraging (ina226_avg_x4);// we turn on the built -in 4 multiple averages, by default there is no averaging

  Serial.println ("");
}

VOID loop () {
  // read tension
  Serial.print ("voltage:"));
  Serial.print (ina.getvoltage (), 3);
  Serial.println ("v"));

  // read the current
  Serial.print ("Current:"));
  Serial.print (ina.getcurrent (), 3);
  Serial.println ("a"));

  // read power
  Serial.print ("Power:"));
  Serial.print (ina.getpower (), 3);
  Serial.println ("w"));

  // Read the voltage in shunt
  Serial.print ("shunt voltage:"));
  Serial.print (ina.getshuntvoltage (), 6);
  Serial.println ("v"));

  Serial.println ("");
  DELAY (1000);
}

`` `
</details>

<a id="versions"> </a>
## versions
- V1.0
- v1.1 - fixed boings, combed the tabulation (alexgyver)
- v1.2 - Fixed error in the calculations ina226
- V1.2.1 - Small corrections of examples, added the ability to set pins i2C for ESP8266/ESP32 in the Begin methods for INA226 and Ina219

<a id="feedback"> </a>Cranberry
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!


When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code