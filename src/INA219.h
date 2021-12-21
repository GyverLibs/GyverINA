#ifndef _INA219_h
#define _INA219_h
#include <Wire.h>
#include <Arduino.h>

/*
Конструктор:	
    INA219 ina219 (Сопротивление шунта, Максимальный ожидаемый ток, Адрес на шине I2c)
    
    INA219 ina219;						// Стандартные значения для модуля INA219 (0.1 Ом, 3.2А, адрес 0x40) - подойдет для одного модуля
    INA219 ina219 (0x41);				// Шунт и макс. ток по умолчанию, адрес 0x41 - подойдет для нескольких модулей
    INA219 ina219 (0.05f);				// Шунт 0.05 Ом, макс. ток и адрес по умолчанию (3.2А, 0x40) - Допиленный модуль или голая м/с
    INA219 ina219 (0.05f, 2.0f);		// Шунт 0.05 Ома, макс. ожидаемый ток 2А, адрес по умолчанию (0x40) - Допиленный модуль или голая м/с
    INA219 ina219 (0.05f, 2.0f, 0x41);	// Шунт 0.05 Ома, макс. ожидаемый ток 2А, адрес 0x41  - Допиленные модули или голые м/с
    
Методы:
    bool begin();							    // Инициализация модуля и проверка присутствия, вернет false если INA219 не найдена
    void sleep(true / false);				    // Включение и выключение режима низкого энергопотребления, в зависимости от аргумента
    void setResolution(channel, mode);		    // Установка разрешения и режима усреднения для измерения напряжения и тока
    
    float getShuntVoltage(); 				    // Прочитать напряжение на шунте
    float getVoltage();	 					    // Прочитать напряжение
    float getCurrent();  					    // Прочитать ток
    float getPower(); 						    // Прочитать мощность
    
    uint16_t getCalibration();	 			    // Прочитать калибровочное значение (после старта рассчитывается автоматически)
    void setCalibration(calibration value);	    // Записать калибровочное значение 	(можно хранить его в EEPROM)	 		
    void adjCalibration(calibration offset);    // Подкрутить калибровочное значение на указанное значение (можно менять на ходу)
*/

/* Public-определения (константы) */   
#define INA219_VBUS           true		// Канал АЦП, измеряющий напряжение шины (0-26в)
#define INA219_VSHUNT         false		// Канал АЦП, измеряющий напряжение на шунте
#define INA219_RES_9BIT       0b0000	// 9 Бит - 84мкс
#define INA219_RES_10BIT      0b0001	// 10 Бит - 148мкс 
#define INA219_RES_11BIT      0b0010	// 11 Бит - 276мкс 
#define INA219_RES_12BIT      0b0011	// 12 Бит - 532мкс
#define INA219_RES_12BIT_X2   0b1001	// 12 Бит, среднее из 2х - 1.06 мс
#define INA219_RES_12BIT_X4   0b1010	// 12 Бит, среднее из 4х - 2.13 мс
#define INA219_RES_12BIT_X8   0b1011	// 12 Бит, среднее из 8х - 4.26 мс
#define INA219_RES_12BIT_X16  0b1100	// 12 Бит, среднее из 16х - 8.51 мс
#define INA219_RES_12BIT_X32  0b1101	// 12 Бит, среднее из 32х - 17.02 мс
#define INA219_RES_12BIT_X64  0b1110	// 12 Бит, среднее из 64х - 34.05 мс
#define INA219_RES_12BIT_X128 0b1111	// 12 Бит, среднее из 128х - 68.10 мс

/* Private-определения (адреса) */
#define INA219_CFG_REG_ADDR   0x00
#define INA219_SHUNT_REG_ADDR 0x01
#define INA219_VBUS_REG_ADDR  0x02
#define INA219_POWER_REG_ADDR 0x03
#define INA219_CUR_REG_ADDR   0x04
#define INA219_CAL_REG_ADDR   0x05

class INA219 {
public:
    // конструктор
    INA219(const float r_shunt = 0.1f, const float i_max = 3.2f, const uint8_t address = 0x40)
    : _r_shunt(r_shunt), _i_max(i_max), _iic_address(address) {}
    
    // конструктор
    INA219(const uint8_t address)
    : _r_shunt(0.1f), _i_max(3.2f), _iic_address(address) {}
    
    // Инициализация и проверка
    bool begin(void) {
        Wire.begin();                           // Инициализация шины I2c
        if (!testConnection()) return false;    // Проверка присутствия
        calibrate();                            // расчёт калибровочного значения и инициализация
        return true;                            // Вернуть true если все ок
    }
    
    // Установка / снятие режима сна
    void sleep(bool state) {
        uint16_t cfg_register = readRegister(INA219_CFG_REG_ADDR) & ~(0b111);			// Прочитать конф. регистр и стереть биты режима
        writeRegister(INA219_CFG_REG_ADDR, cfg_register | (state ? 0b000 : 0b111));	// Записать новое значение конф. регистра с выбранным режимом
    }
    
    // Подстройка калибровочного значения
    void adjCalibration(int16_t adj) {
        setCalibration(getCalibration() + adj);     // Читаем и модифицируем значение
        _cal_value = _cal_value + adj;              // Обновляем внутреннюю переменную
    }	
    
    // Установка калибровочного значения 
    void setCalibration(uint16_t cal) {
        writeRegister(INA219_CAL_REG_ADDR, cal);    // Пишем значение в регистр калибровки
        _cal_value = cal;                           // Обновляем внутреннюю переменную 
    }
    
    // Чтение калибровочного значения 
    uint16_t getCalibration(void) {
        _cal_value = readRegister(INA219_CAL_REG_ADDR); // Обновляем внутреннюю переменну
        return _cal_value;                              // Возвращаем значение
    }
    
    // Установка разрешения для выбранного канала
    void setResolution(bool ch, uint8_t mode) {
        uint16_t cfg_register = readRegister(INA219_CFG_REG_ADDR);      // Читаем конф. регистр
        cfg_register &= ~ ((0b1111) << (ch ? 7 : 3));                   // Сбрасываем нужную пачку бит, в зависимости от канала
        cfg_register |= mode << (ch ? 7 : 3);                           // Пишем нужную пачку бит, в зависимости от канала
        writeRegister(INA219_CFG_REG_ADDR, cfg_register);               // Пишем новое значение конф. регистра
    }
    
    // Чтение напряжения на шунте
    float getShuntVoltage(void) {
        setCalibration(_cal_value);                             // Принудительное обновление калибровки (на случай внезапного ребута INA219)
        int16_t value = readRegister(INA219_SHUNT_REG_ADDR);    // Чтение регистра напряжения шунта
        return value * 0.00001f;                                // LSB = 10uV = 0.00001V, умножаем и возвращаем
    }
    
    // Чтение напряжения 
    float getVoltage(void) {
        uint16_t value = readRegister(INA219_VBUS_REG_ADDR);    // Чтение регистра напряжения 
        return (value >> 3) * 0.004f;                           // LSB = 4mV = 0.004V, Сдвигаем значение до 12 бит и умножаем 
    }
    
    // Чтение тока
    float getCurrent(void) {
        setCalibration(_cal_value);                             // Принудительное обновление калибровки (на случай внезапного ребута INA219)
        int16_t value = readRegister(INA219_CUR_REG_ADDR);      // Чтение регистра тока
        return value * _current_lsb;                            // LSB рассчитывается на основе макс. ожидаемого тока, умножаем и возвращаем
    }	
    
    // Чтение мощности 
    float getPower(void) {
        setCalibration(_cal_value);                             // Принудительное обновление калибровки (на случай внезапного ребута INA219)
        uint16_t value = readRegister(INA219_POWER_REG_ADDR);   // Чтение регистра мощности
        return value * _power_lsb;                              // LSB в 20 раз больше LSB для тока, умножаем возвращаем
    }

private:
    const uint8_t _iic_address = 0x00;  // Адрес на шине I2c					
    const float _r_shunt = 0.0;         // Сопротивление шунта	
    const float _i_max = 0.0;           // Макс. ожидаемый ток

    float _current_lsb = 0.0;           // LSB для тока
    float _power_lsb = 0.0;             // LSB для мощности
    uint16_t _cal_value = 0;            // Калибровочное значение
    
    // Запись 16-ти битного регистра INA219
    void writeRegister(uint8_t address, uint16_t data) {
        Wire.beginTransmission(_iic_address);               // Начинаем передачу
        Wire.write(address);                                // Отправляем адрес
        Wire.write(highByte(data));                         // Отправляем старший байт
        Wire.write(lowByte(data));                          // Отправляем младший байт
        Wire.endTransmission();                             // Заканчиваем передачу
    }
    
    // Чтение 16-ти битного регистра INA219
    uint16_t readRegister(uint8_t address) {
        Wire.beginTransmission(_iic_address);       // Начинаем передачу
        Wire.write(address);                        // Отправляем адрес	
        Wire.endTransmission();                     // Заканчиваем передачу
        Wire.requestFrom(_iic_address, (uint8_t)2); // Запрашиваем 2 байта
        return Wire.read() << 8 | Wire.read();      // Клеим и возвращаем результат
    }
    
    // Проверка присутствия
    bool testConnection(void) {
        Wire.beginTransmission(_iic_address);       // Начинаем передачу 
        return (bool)!Wire.endTransmission();       // Сразу заканчиваем, инвертируем результат
    }
    
    // Процедура рассчета калибровочного значения и инициализации
    void calibrate(void) {
        writeRegister(INA219_CFG_REG_ADDR, 0x8000);                 // Принудительный сброс

        _current_lsb = _i_max / 32768.0f;                           // Рассчет LSB для тока (см. доку INA219)
        _power_lsb = _current_lsb * 20.0f;                          // Рассчет LSB для мощности (см. доку INA219)
        _cal_value = trunc(0.04096f / (_current_lsb * _r_shunt));   // Рассчет калибровочного значения (см. доку INA219)

        setCalibration(_cal_value);                                 // Записываем стандартное калибровочное значение

        uint16_t cfg_register = readRegister(INA219_CFG_REG_ADDR) & ~(0b11 << 11);  // Читаем регистр калибровки, сбрасываем биты диапазона напряжения шунта
        uint16_t vshunt_max_mv = (_r_shunt * _i_max) * 1000;        // Считаем макс напряжение на шунте при макс ожидаемом токе

        if (vshunt_max_mv <= 40) cfg_register |= 0b00 << 11;        // +/- 40mV  - выбираем диапазон
        else if (vshunt_max_mv <= 80) cfg_register |= 0b01 << 11;   // +/- 80mV
        else if (vshunt_max_mv <= 160) cfg_register |= 0b10 << 11;  // +/- 160mV
        else cfg_register |= 0b11 << 11;                            // +/- 320mV
        writeRegister(INA219_CFG_REG_ADDR, cfg_register);           // Пишем конф. регистр
    }
};
#endif