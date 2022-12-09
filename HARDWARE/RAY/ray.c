#include "led.h"
#include "exti.h"
#include "i2c.h"
#include "uart_print_loop.h"
#include "delay.h"
#include "ray.h"
void	i2c_data_read(I2C_TypeDef* i2c,uint8_t regbuf,uint8_t *rxbuf,uint8_t len)
{
	int i;
	I2C_SendData(i2c,regbuf);
	DELAY_Ms(1);
	for(i=0;i<len;i++)
	{
		I2C_ReadCmd(i2c);
		DELAY_Ms(1);
		rxbuf[i]=I2C_ReceiveData(i2c);
	}
	I2C_GenerateSTOP(I2C1,ENABLE);
	DELAY_Ms(1);
}
