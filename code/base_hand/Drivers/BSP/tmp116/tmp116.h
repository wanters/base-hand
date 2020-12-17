#include "stm32f4xx_hal.h"

//#include "debug.h"
//#include "lwip/debug.h"
//#include "delay.h"


void I2C1_ResetAll(void);

void GetTemperature(void);
void Continuous(void);

float gettmp116(void);
void force_shutdown(void);

