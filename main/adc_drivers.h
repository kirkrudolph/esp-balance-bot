#ifndef ADC_DRIVERS_H 
#define ADC_DRIVERS_H

#define BATTERY_5V_PIN 36       // GPIO (Note: ADC1_CHANNEL_0 is GPIO 36)
#define R1 8.2                  // Resistor in kOhm
#define R2 2.0                  // Resistor in kOhm

void init_adc_task(void *params);
void print_adc_data_task(void *params);

#endif // ADC_DRIVERS_H