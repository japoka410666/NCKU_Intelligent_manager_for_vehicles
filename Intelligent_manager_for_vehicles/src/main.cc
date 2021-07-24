/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "main_functions.h"
#include "hx_drv_tflm.h"
#include "stdio.h"
#include "synopsys_wei_gpio.h"

volatile void delay_ms(uint32_t ms_input);

#define accel_scale 10

typedef struct
{
	uint8_t symbol;
	uint32_t int_part;
	uint32_t frac_part;
} accel_type;

char string_buf[100] = "test\n";

hx_drv_gpio_config_t hal_gpio_1;
void GPIO_INIT(void);


int main(int argc, char* argv[]) {
	int result2 = 0;
	int32_t count = 0;
	int32_t unknown_person_count = 0;
	int32_t int_buf;
	accel_type accel_x, accel_y, accel_z, accel_g;

	hx_drv_uart_initial(UART_BR_115200);
	GPIO_INIT();
  

	if (hx_drv_accelerometer_initial() != HX_DRV_LIB_PASS)
		hx_drv_uart_print("Accelerometer Initialize Fail\n");
	else
		hx_drv_uart_print("Accelerometer Initialize Success\n");

  	setup();
  	while (true) 
    {


   		 hx_drv_led_off(HX_DRV_LED_RED);
    

		uint32_t available_count = 0;
		float x, y, z , g;
		available_count = hx_drv_accelerometer_available_count();				
		hx_drv_uart_print("Accel get FIFO: %d\n", available_count);
		for (int i = 0; i < available_count; i++) 
		{
			hx_drv_accelerometer_receive(&x, &y, &z);
		}

		int_buf = x * accel_scale; //scale value
		if(int_buf < 0)
		{
			int_buf = int_buf * -1;
			accel_x.symbol = '-';
		}
		else 
		{
			accel_x.symbol = '+';
		}
		accel_x.int_part = int_buf / accel_scale;
		accel_x.frac_part = int_buf % accel_scale;


		int_buf = y * accel_scale; //scale value
		if(int_buf < 0)
		{
			int_buf = int_buf * -1;
			accel_y.symbol = '-';
		}
		else 
		{
			accel_y.symbol = '+';
		}
		accel_y.int_part = int_buf / accel_scale;
		accel_y.frac_part = int_buf % accel_scale;


		int_buf = z * accel_scale; //scale value
		if(int_buf < 0)
		{
			int_buf = int_buf * -1;
			accel_z.symbol = '-';
		}
		else 
		{
			accel_z.symbol = '+';
		}
		accel_z.int_part = int_buf / accel_scale;
		accel_z.frac_part = int_buf % accel_scale;

    	g=x*x+y*y+z*z;

    	int_buf =g * accel_scale;
    	if(int_buf < 0)
    	{
			int_buf = int_buf * -1;
			accel_g.symbol = '-';
		}
		else 
		{
			accel_g.symbol = '+';
		}
    
    	accel_g.int_part = int_buf / accel_scale;
		accel_g.frac_part = int_buf % accel_scale;

		sprintf(string_buf, "%c%1d.%1d | %c%1d.%1d | %c%1d.%1d | %c%1d.%1d G\n", 
				accel_x.symbol, accel_x.int_part, accel_x.frac_part, 
				accel_y.symbol, accel_y.int_part, accel_y.frac_part, 
				accel_z.symbol, accel_z.int_part, accel_z.frac_part,
        accel_g.symbol, accel_g.int_part, accel_g.frac_part);
		hx_drv_uart_print(string_buf);


		hx_drv_uart_print("\n\n");

		delay_ms(100);
    
		//detect somebody boarding
    	while (g >= 1.5)
    	{
			hx_drv_led_off(HX_DRV_LED_RED);

			result2=eyes_loop();//call eyes model

			if(result2== 3 )
			{
				count = 0;
				hx_drv_uart_print("WARNING: Unkown Person\n");
				unknown_person_count++;
				if(unknown_person_count >= 5)
				{
				//********unknown person in your car**********//
				//********return to line            **********//
				hal_gpio_set(&hal_gpio_1, GPIO_PIN_SET);
				///*******************************************//
				}
			}
			else 
			{
				unknown_person_count = 0;
				hal_gpio_set(&hal_gpio_1, GPIO_PIN_RESET);
				if(result2 == 0)			//eyes close
				{
				count++;
				}
				else if(result2 == 2)		//not susre
				{
				count = count;
				}
				else						//eyes open
				{
				count = 0;
				}

				//if eyes closing time too long, warning by red LED
				if (count >= 5)
				{
				hx_drv_led_on(HX_DRV_LED_RED);
				hx_drv_uart_print("WARNING: You are too tired now\n");
				delay_ms(1500);
				count = 0;
				}
			}
			delay_ms(500);
			
		}
    }
}
volatile void delay_ms(uint32_t ms_input)
{
    volatile uint32_t i = 0;
    volatile uint32_t j = 0;

    for(i = 0; i < ms_input; i++)
        for(j = 0; j < 40000; j++);
}

void GPIO_INIT(void)
{
  if(hal_gpio_init(&hal_gpio_1, HX_DRV_PGPIO_1, HX_DRV_GPIO_OUTPUT, GPIO_PIN_RESET) == HAL_OK)
    hx_drv_uart_print("GPIO2 Initialized: OK\n");
  else
    hx_drv_uart_print("GPIO2 Initialized: Error\n");
}

