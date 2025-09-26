#pragma once

#include "driver/i2c_master.h"
#include "esp_log.h"
#include "esp_err.h"

// AW32001 I2C 从机地址（7位，二进制1001001，十六进制0x49）
#define AW32001_I2C_ADDR 0x49
// I2C 超时时间（单位：ms）
#define AW32001_I2C_TIMEOUT_MS 1000

// AW32001 寄存器地址定义
#define AW32001_REG_00H 0x00 // 输入源控制寄存器
#define AW32001_REG_01H 0x01 // 上电配置寄存器
#define AW32001_REG_02H 0x02 // 充电电流控制寄存器
#define AW32001_REG_03H 0x03 // 放电电流控制寄存器
#define AW32001_REG_04H 0x04 // 充电电压寄存器
#define AW32001_REG_05H 0x05 // 充电终止/定时器控制寄存器
#define AW32001_REG_06H 0x06 // 主控制寄存器
#define AW32001_REG_07H 0x07 // 系统电压控制寄存器
#define AW32001_REG_08H 0x08 // 系统状态寄存器
#define AW32001_REG_09H 0x09 // 故障寄存器
#define AW32001_REG_0AH 0x0A // 芯片ID寄存器
#define AW32001_REG_0BH 0x0B // 独立充电寄存器
#define AW32001_REG_0CH 0x0C // 附加功能控制寄存器
#define AW32001_REG_22H 0x22 // 附加功能控制寄存器1

// 充电状态枚举
typedef enum
{
    AW32001_CHG_STAT_NOT_CHARGING = 0x00, // 未充电
    AW32001_CHG_STAT_PRE_CHARGE = 0x01,   // 预充电
    AW32001_CHG_STAT_FAST_CHARGE = 0x02,  // 快速充电
    AW32001_CHG_STAT_CHARGE_DONE = 0x03   // 充电完成
} aw32001_chg_stat_t;

// 系统状态结构体
typedef struct
{
    bool watchdog_fault;         // 看门狗故障（1：故障，0：正常）
    aw32001_chg_stat_t chg_stat; // 充电状态
    bool ppm_stat;               // PPM状态（1：IN PPM，0：无PPM）
    bool pg_stat;                // 电源状态（1：电源正常，0：电源故障）
    bool therm_stat;             // 热调节状态（1：热调节中，0：无热调节）
} aw32001_sys_status_t;

// 故障状态结构体
typedef struct
{
    bool vin_fault;      // 输入故障（1：故障，0：正常）
    bool therm_sd;       // 热关断（1：关断，0：正常）
    bool bat_fault;      // 电池过压故障（1：故障，0：正常）
    bool stmr_fault;     // 安全定时器故障（1：故障，0：正常）
    bool ntc_fault_high; // NTC高温故障（1：故障，0：正常）
    bool ntc_fault_low;  // NTC低温故障（1：故障，0：正常）
} aw32001_fault_status_t;

extern aw32001_sys_status_t pwr_sys_status;

esp_err_t aw32001_init(i2c_master_bus_handle_t bus_handle);
esp_err_t aw32001_read_reg(uint8_t reg_addr, uint8_t *reg_val);
esp_err_t aw32001_write_reg(uint8_t reg_addr, uint8_t reg_val);
esp_err_t aw32001_disable_watchdog();
esp_err_t aw32001_set_chg_current(uint16_t chg_current);
esp_err_t aw32001_get_chg_current(uint16_t *chg_current);
esp_err_t aw32001_set_chg_voltage(float chg_voltage);
esp_err_t aw32001_set_dischg_current(uint16_t dischg_current);
esp_err_t aw32001_enable_charge();
esp_err_t aw32001_disable_charge();
esp_err_t aw32001_read_sys_status(aw32001_sys_status_t *sys_status);
esp_err_t aw32001_read_fault_status(aw32001_fault_status_t *fault_status);
esp_err_t aw32001_enable_ntc();
esp_err_t aw32001_disable_ntc();
esp_err_t aw32001_enter_shipping_mode();
esp_err_t aw32001_set_vsys_reg(float sys_reg_voltage);
void aw32001_interrupt_init();
