#include "mpu6050.h"
#include "sys.h"
#include "Delay.h"
#include "mpuiic.h"
#include "general.h"
#include "Serial.h"

// 初始化MPU6050
// 返回值:0,成功
// 其他,错误代码
uint8_t MPU_Init(void)
{
    uint8_t res;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 使能AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 先使能外设IO PORTA时钟

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;      // 端口配置
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);            // 根据设定参数初始化GPIOA

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // 禁止JTAG,从而PA15可以做普通IO使用,否则PA15不能做普通IO!!!

    MPU_AD0_CTRL = 0; // 控制MPU6050的AD0脚为低电平,从机地址为:0X68

    MPU_IIC_Init();                          // 初始化MPUIIC总线
    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80); // 复位MPU6050                   设置电源管理寄存器 1（0X6B）的bit7 为 1
    Delay_ms(100);
    MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00); // 唤醒MPU6050                   设置电源管理寄存器 1（0X6B）为 0X00
    MPU_Set_Gyro_Fsr(3);                     // 陀螺仪传感器,±2000dps                   设置陀螺仪配置寄存器（0X1B）bit4、bit3 为 3
    MPU_Set_Accel_Fsr(0);                    // 加速度传感器,±2g                        设置加速度传感器配置寄存器（0X1C）bit4、bit3 为 0
    MPU_Set_Rate(50);                        // 设置采样率50Hz                            F12
    MPU_Write_Byte(MPU_INT_EN_REG, 0X00);    // 关闭所有中断
    MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00); // I2C主模式关闭
    MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);   // 关闭FIFO                        设置FIFO 使能寄存器（0X23）为 0X00
    MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80); // INT引脚低电平有效
    res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
    if (res == MPU_ADDR) // 器件ID正确
    {
        MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01); // 设置CLKSEL,PLL X轴为参考    设置电源管理寄存器 1（0X1B）bit2、bit1、bit0 为 001
        MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00); // 加速度与陀螺仪都工作        设置电源管理寄存器2（0X6C） 为 0X00
        MPU_Set_Rate(50);                        // 设置采样率为50Hz
    } else
        return 1;
    return 0;
}
// 以上为MPU6050初始化部分
// 以下为MPU6050初始化调用的函数

// 设置MPU6050陀螺仪传感器满量程范围
// fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr << 3); // 设置陀螺仪满量程范围
}
// 设置MPU6050加速度传感器满量程范围
// fsr:0,±2g;1,±4g;2,±8g;3,±16g
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr << 3); // 设置加速度传感器满量程范围
}
// 设置MPU6050的数字低通滤波器
// lpf:数字低通滤波频率(Hz)
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU_Set_LPF(u16 lpf)
{
    uint8_t data = 0;
    if (lpf >= 188)
        data = 1;
    else if (lpf >= 98)
        data = 2;
    else if (lpf >= 42)
        data = 3;
    else if (lpf >= 20)
        data = 4;
    else if (lpf >= 10)
        data = 5;
    else
        data = 6;
    return MPU_Write_Byte(MPU_CFG_REG, data); // 设置数字低通滤波器
}
// 设置MPU6050的采样率(假定陀螺仪输出频率Fs=1KHz)
// rate:4~1000(Hz)
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU_Set_Rate(u16 rate)
{
    uint8_t data;
    if (rate > 1000) rate = 1000;
    if (rate < 4) rate = 4;
    data = 1000 / rate - 1;                           // 由采样频率 = 陀螺仪输出频率 / (1+SMPLRT_DIV)得 SMPLRT_DIV= 陀螺仪输出频率 / 采样频率 - 1
    data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data); // 设置数字低通滤波器
    return MPU_Set_LPF(rate / 2);                     // 自动设置LPF为采样率的一半
}

// 得到温度值
// 返回值:温度值(扩大了100倍)
uint8_t MPU_Get_Temperature(short *temperature)
{
    uint8_t buf[2], res;
    short raw;
    float temp;
    res = MPU_Read_Len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
    if (res == 0) {
        raw          = ((u16)buf[0] << 8) | buf[1];
        temp         = 36.53 + ((double)raw) / 340;
        *temperature = temp * 100;
    }
    return res;
}
/* rt_err_t mpu6050_temperature_get(rt_int16_t *temperature) */
/* { */
/*     rt_uint8_t buf[2];  */
/*     rt_int16_t raw; */
/*     float temp; */
/*     rt_err_t ret; */
/**/
/*     ret = mpu6050_read_reg(MPU_TEMP_OUTH_REG, 2, buf); */
/*     if (ret == RT_EOK)  */
/*     {         */
/*         raw = ((rt_uint16_t)buf[0] << 8) | buf[1]; */
/*         temp = 36.53 + ((double)raw)/340; */
/*         *temperature = temp*100; */
/**/
/*         return RT_EOK; */
/*     } */
/*     else */
/*     { */
/*         return -RT_ERROR; */
/*     } */
/* } */

// 得到陀螺仪值(原始值)
// gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
// 返回值:0,成功
//     其他,错误代码
uint8_t MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    uint8_t buf[6], res;
    res = MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
    if (res == 0) {
        *gx = ((u16)buf[0] << 8) | buf[1];
        *gy = ((u16)buf[2] << 8) | buf[3];
        *gz = ((u16)buf[4] << 8) | buf[5];
    }
    return res;
}
// 得到加速度值(原始值)
// gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
// 返回值:0,成功
//     其他,错误代码
uint8_t MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
    uint8_t buf[6], res;
    res = MPU_Read_Len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
    if (res == 0) {
        *ax = ((u16)buf[0] << 8) | buf[1];
        *ay = ((u16)buf[2] << 8) | buf[3];
        *az = ((u16)buf[4] << 8) | buf[5];
    }
    return res;
}
// IIC连续写
// addr:器件地址
// reg:寄存器地址
// len:写入长度
// buf:数据区
// 返回值:0,正常
//     其他,错误代码
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    uint8_t i;
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr << 1) | 0); // 发送器件地址+写命令
    if (MPU_IIC_Wait_Ack())             // 等待应答
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg); // 写寄存器地址
    MPU_IIC_Wait_Ack();     // 等待应答
    for (i = 0; i < len; i++) {
        MPU_IIC_Send_Byte(buf[i]); // 发送数据
        if (MPU_IIC_Wait_Ack())    // 等待ACK
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_IIC_Stop();
    return 0;
}
// IIC连续读
// addr:器件地址
// reg:要读取的寄存器地址
// len:要读取的长度
// buf:读取到的数据存储区
// 返回值:0,正常
//     其他,错误代码
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr << 1) | 0); // 发送器件地址+写命令
    if (MPU_IIC_Wait_Ack())             // 等待应答
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg); // 写寄存器地址
    MPU_IIC_Wait_Ack();     // 等待应答
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr << 1) | 1); // 发送器件地址+读命令
    MPU_IIC_Wait_Ack();                 // 等待应答
    while (len) {
        if (len == 1)
            *buf = MPU_IIC_Read_Byte(0); // 读数据,发送nACK
        else
            *buf = MPU_IIC_Read_Byte(1); // 读数据,发送ACK
        len--;
        buf++;
    }
    MPU_IIC_Stop(); // 产生一个停止条件
    return 0;
}
// IIC写一个字节
// reg:寄存器地址
// data:数据
// 返回值:0,正常
//     其他,错误代码
uint8_t MPU_Write_Byte(uint8_t reg, uint8_t data)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR << 1) | 0); // 发送器件地址+写命令
    if (MPU_IIC_Wait_Ack())                 // 等待应答
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg);  // 写寄存器地址
    MPU_IIC_Wait_Ack();      // 等待应答
    MPU_IIC_Send_Byte(data); // 发送数据
    if (MPU_IIC_Wait_Ack())  // 等待ACK
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Stop();
    return 0;
}
// IIC读一个字节
// reg:寄存器地址
// 返回值:读到的数据
uint8_t MPU_Read_Byte(uint8_t reg)
{
    uint8_t res;
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR << 1) | 0); // 发送器件地址+写命令
    MPU_IIC_Wait_Ack();                     // 等待应答
    MPU_IIC_Send_Byte(reg);                 // 写寄存器地址
    MPU_IIC_Wait_Ack();                     // 等待应答
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR << 1) | 1); // 发送器件地址+读命令
    MPU_IIC_Wait_Ack();                     // 等待应答
    res = MPU_IIC_Read_Byte(0);             // 读取数据,发送nACK
    MPU_IIC_Stop();                         // 产生一个停止条件
    return res;
}
