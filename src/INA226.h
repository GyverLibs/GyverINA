#ifndef _INA226_h
#define _INA226_h
#include <Arduino.h>
#include <Wire.h>

/*
Конструктор:
    INA226 ina226 (Сопротивление шунта, Максимальный ожидаемый ток, Адрес на шине I2c)

    INA226 ina226;						// Стандартные значения для модуля INA226 (0.1 Ом, 0.8 А, адрес 0x40) - подойдет для одного модуля
    INA226 ina226 (0x41);				// Шунт и макс. ток по умолчанию, адрес 0x41 - подойдет для нескольких модулей
    INA226 ina226 (0.05f);				// Шунт 0.05 Ом, макс. ток и адрес по умолчанию (0.8 А, 0x40) - Допиленный модуль или голая м/с
    INA226 ina226 (0.05f, 1.5f);		// Шунт 0.05 Ома, макс. ожидаемый ток 1.5 А, адрес по умолчанию (0x40) - Допиленный модуль или голая м/с
    INA226 ina226 (0.05f, 1.5f, 0x41);	// Шунт 0.05 Ома, макс. ожидаемый ток 1.5 А, адрес 0x41  - Допиленные модули или голые м/с

Методы:
        bool begin();							    // Инициализация модуля и проверка присутствия, вернет false если INA226 не найдена
        bool begin(SCL, SDA);						// Инициализация модуля для ESP8266/ESP32
    void sleep(true / false);				    // Включение и выключение режима низкого энергопотребления, в зависимости от аргумента
    void setAveraging(avg);					    // Установка количества усреднений измерений (см. таблицу ниже)
    void setSampleTime(ch, time);			    // Установка времени выборки напряжения и тока (INA226_VBUS / INA226_VSHUNT), по умолчанию INA226_CONV_1100US

    float getShuntVoltage(); 				    // Прочитать напряжение на шунте
    float getVoltage();	 					    // Прочитать напряжение
    float getCurrent();  					    // Прочитать ток
    float getPower(); 						    // Прочитать мощность

    uint16_t getCalibration();	 			    // Прочитать калибровочное значение (после старта рассчитывается автоматически)
    void setCalibration(calibration value);	    // Записать калибровочное значение 	(можно хранить его в EEPROM)
    void adjCalibration(calibration offset);    // Подкрутить калибровочное значение на указанное значение (можно менять на ходу)

    Версия 1.0 от 31.10.2021
*/

/* Public-определения (константы) */
#define INA226_VBUS true     // Канал АЦП, измеряющий напряжение шины (0-36в)
#define INA226_VSHUNT false  // Канал АЦП, измеряющий напряжение на шунте

#define INA226_CONV_140US 0b000  // Время выборки (накопления сигнала для оцифровки)
#define INA226_CONV_204US 0b001
#define INA226_CONV_332US 0b010
#define INA226_CONV_588US 0b011
#define INA226_CONV_1100US 0b100
#define INA226_CONV_2116US 0b101
#define INA226_CONV_4156US 0b110
#define INA226_CONV_8244US 0b111

#define INA226_AVG_X1 0b000  // Встроенное усреднение (пропорционально увеличивает время оцифровки)
#define INA226_AVG_X4 0b001
#define INA226_AVG_X16 0b010
#define INA226_AVG_X64 0b011
#define INA226_AVG_X128 0b100
#define INA226_AVG_X256 0b101
#define INA226_AVG_X512 0b110
#define INA226_AVG_X1024 0b111

/* Private-определения (адреса) */
#define INA226_CFG_REG_ADDR 0x00
#define INA226_SHUNT_REG_ADDR 0x01
#define INA226_VBUS_REG_ADDR 0x02
#define INA226_POWER_REG_ADDR 0x03
#define INA226_CUR_REG_ADDR 0x04
#define INA226_CAL_REG_ADDR 0x05

class INA226 {
   public:
    INA226(const float r_shunt = 0.1f, const float i_max = 0.8f, const uint8_t address = 0x40)
        : _r_shunt(r_shunt), _i_max(i_max), _iic_address(address) {}

    INA226(const uint8_t address)
        : _r_shunt(0.1f), _i_max(0.8f), _iic_address(address) {}

    // Инициализация и проверка
    bool begin(int __attribute__((unused)) sda = 0, int __attribute__((unused)) scl = 0) {
#if defined(ESP32) || defined(ESP8266)
        if (sda || scl) Wire.begin(sda, scl);  // Инициализация для ESP
        else Wire.begin();
#else
        Wire.begin();  // Инициализация шины I2c
#endif
        if (!testConnection()) return false;  // Проверка присутствия
        calibrate();                          // расчёт калибровочного значения и инициализация
        return true;                          // Вернуть true если все ок
    }

    // Установка / снятие режима сна
    void sleep(bool state) {
        uint16_t cfg_register = readRegister(INA226_CFG_REG_ADDR) & ~(0b111);        // Прочитать конф. регистр и стереть биты режима
        writeRegister(INA226_CFG_REG_ADDR, cfg_register | (state ? 0b000 : 0b111));  // Записать новое значение конф. регистра с выбранным режимом
    }

    // Подстройка калибровочного значения
    void adjCalibration(int16_t adj) {
        setCalibration(getCalibration() + adj);  // Читаем и модифицируем значение
        _cal_value = _cal_value + adj;           // Обновляем внутреннюю переменную
    }

    // Установка калибровочного значения
    void setCalibration(uint16_t cal) {
        writeRegister(INA226_CAL_REG_ADDR, cal);  // Пишем значение в регистр калибровки
        _cal_value = cal;                         // Обновляем внутреннюю переменную
    }

    // Чтение калибровочного значения
    uint16_t getCalibration(void) {
        _cal_value = readRegister(INA226_CAL_REG_ADDR);  // Обновляем внутреннюю переменну
        return _cal_value;                               // Возвращаем значение
    }

    // Установка встроенного усреднения выборок
    void setAveraging(uint8_t avg) {
        uint16_t cfg_register = readRegister(INA226_CFG_REG_ADDR) & ~(0b111 << 9);  // Читаем конф. регистр, сбросив биты AVG2-0
        writeRegister(INA226_CFG_REG_ADDR, cfg_register | avg << 9);                // Пишем новое значение конф. регистр
    }

    // Установка разрешения для выбранного канала
    void setSampleTime(bool ch, uint8_t mode) {
        uint16_t cfg_register = readRegister(INA226_CFG_REG_ADDR);  // Читаем конф. регистр
        cfg_register &= ~((0b111) << (ch ? 6 : 3));                 // Сбрасываем нужную пачку бит, в зависимости от канала
        cfg_register |= mode << (ch ? 6 : 3);                       // Пишем нужную пачку бит, в зависимости от канала
        writeRegister(INA226_CFG_REG_ADDR, cfg_register);           // Пишем новое значение конф. регистра
    }

    // Чтение напряжения на шунте
    float getShuntVoltage(void) {
        setCalibration(_cal_value);                           // Принудительное обновление калибровки (на случай внезапного ребута INA219)
        int16_t value = readRegister(INA226_SHUNT_REG_ADDR);  // Чтение регистра напряжения шунта
        return value * 0.0000025f;                            // LSB = 2.5uV = 0.0000025V, умножаем и возвращаем
    }

    // Чтение напряжения
    float getVoltage(void) {
        uint16_t value = readRegister(INA226_VBUS_REG_ADDR);  // Чтение регистра напряжения
        return value * 0.00125f;                              // LSB = 1.25mV = 0.00125V, Сдвигаем значение до 12 бит и умножаем
    }

    // Чтение тока
    float getCurrent(void) {
        setCalibration(_cal_value);                         // Принудительное обновление калибровки (на случай внезапного ребута INA219)
        int16_t value = readRegister(INA226_CUR_REG_ADDR);  // Чтение регистра тока
        return value * _current_lsb;                        // LSB рассчитывается на основе макс. ожидаемого тока, умножаем и возвращаем
    }

    // Чтение мощности
    float getPower(void) {
        setCalibration(_cal_value);                            // Принудительное обновление калибровки (на случай внезапного ребута INA219)
        uint16_t value = readRegister(INA226_POWER_REG_ADDR);  // Чтение регистра мощности
        return value * _power_lsb;                             // LSB в 25 раз больше LSB для тока, умножаем возвращаем
    }

   private:
    const float _r_shunt = 0.0;         // Сопротивление шунта
    const float _i_max = 0.0;           // Макс. ожидаемый ток
    const uint8_t _iic_address = 0x00;  // Адрес на шине I2c

    float _current_lsb = 0.0;  // LSB для тока
    float _power_lsb = 0.0;    // LSB для мощности
    uint16_t _cal_value = 0;   // Калибровочное значение

    // Запись 16-ти битного регистра INA219
    void writeRegister(uint8_t address, uint16_t data) {
        Wire.beginTransmission(_iic_address);  // Начинаем передачу
        Wire.write(address);                   // Отправляем адрес
        Wire.write(highByte(data));            // Отправляем старший байт
        Wire.write(lowByte(data));             // Отправляем младший байт
        Wire.endTransmission();                // Заканчиваем передачу
    }

    // Чтение 16-ти битного регистра INA219
    uint16_t readRegister(uint8_t address) {
        Wire.beginTransmission(_iic_address);        // Начинаем передачу
        Wire.write(address);                         // Отправляем адрес
        Wire.endTransmission();                      // Заканчиваем передачу
        Wire.requestFrom(_iic_address, (uint8_t)2);  // Запрашиваем 2 байта
        return Wire.read() << 8 | Wire.read();       // Клеим и возвращаем результат
    }

    // Проверка присутствия
    bool testConnection(void) {
        Wire.beginTransmission(_iic_address);  // Начинаем передачу
        return (bool)!Wire.endTransmission();  // Сразу заканчиваем, инвертируем результат
    }

    // Процедура расчёта калибровочного значения и инициализации
    void calibrate(void) {
        writeRegister(INA226_CFG_REG_ADDR, 0x8000);  // Принудительный сброс

        _current_lsb = _i_max / 32768.0f;                          // расчёт LSB для тока (см. доку INA219)
        _power_lsb = _current_lsb * 25.0f;                         // расчёт LSB для мощности (см. доку INA219)
        _cal_value = trunc(0.00512f / (_current_lsb * _r_shunt));  // расчёт калибровочного значения (см. доку INA219)

        setCalibration(_cal_value);  // Записываем стандартное калибровочное значение
    }
};
#endif
