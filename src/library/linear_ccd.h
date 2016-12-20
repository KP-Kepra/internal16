#include "adc.h"
#include "stm32f10x.h"
#include "lcd_blue.h"
#include "ticks.h"
#include "motor.h"
#include "savgol.h"
#include <math.h>

extern u32 linear_ccd_buffer1[128];
extern u32 linear_ccd_buffer2[128];

#define CLK_PORT GPIOA
#define CLK_PIN GPIO_Pin_8

#define SI1_PORT GPIOA
#define SI1_PIN GPIO_Pin_0

#define SI2_PORT GPIOB
#define SI2_PIN GPIO_Pin_0

#define AO1_channel 1
#define AO2_channel 2

void linear_ccd_init(void);
void linear_ccd_read(void);
void linear_ccd_print(void);
void linear_ccd_clear(void);
