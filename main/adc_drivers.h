#ifndef ADC_DRIVERS_H 
#define ADC_DRIVERS_H

#define BATTERY_5V_PIN ADC1_CHANNEL_0_GPIO_NUM      // GPIO (Note: ADC1_CHANNEL_0 is GPIO 36)
#define R1 8200                                     // Resistor in Ohm
#define R2 2000                                     // Resistor in Ohm

void init_adc_task(void *params);
void print_adc_data_task(void *params);

#endif // ADC_DRIVERS_H