#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "reg_i2c.h"
void	i2c_data_read(I2C_TypeDef* i2c,uint8_t regbuf,uint8_t *rxbuf,uint8_t len);
