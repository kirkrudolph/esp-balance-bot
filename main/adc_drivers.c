#include "adc_drivers.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "ADC_DRIVERS"

/* Google "Voltage Divider Calculator" (V_out = V_ref*R2 / (R1 + R2))
*
*       -----------------  V_ref (+5.61V)                                    
*       |               |                          
*       |               \                          
*       |  R1(8.2kOhm)  /                           
*       |               \                          
*       |               |                          
*      ---              |-----------o V_out (1.1V)
*       -               |                          
*       |               \               
*       |  R2(2.0kOhm)  /     
*       |               \     
*       |               |
*       ----------------------------o GND
*
*   5.0V / (R1+R2)*(1000*1000)  = 490uA bleed current
*   1.1V * (R1+R2)/R2           = 5.61V Max Detect
*   5.6V / 4095 * (1000)        = 1.37 mV LSB
*
*/

float get_5V_bus_voltage_V(void){
    int val = adc1_get_raw(ADC1_CHANNEL_0);                             // Note: Independent of attenuation  
    float Vout = val/4095.0;                                            // Convert to 1.1V. Using 12bit ADC.
    float Vref = Vout*(R1+R2)/R2;                                       // Calculate 5V bus.
    return Vref;
}

float get_voltage_sensor_bleed_current_uA(void){
    float Vref = get_5V_bus_voltage_V();
    float uA = Vref/(R1+R2)*1000*1000;
    return uA;
}

float get_voltage_sensor_bleed_power_mW(void){
    float Vref_V = get_5V_bus_voltage_V();
    float uA = get_voltage_sensor_bleed_current_uA();
    float mW = Vref_V*uA/1000;
    return mW;
}

void print_voltage_sensor_quantization(void){
    float Vmax_V = 1.1*(R1+R2)/R2;                                      // Constant: Dependent on attenuation
    float LSB_uV = Vmax_V / 4095 * (1000);                              // Constant: Dependent on attenuation
    ESP_LOGI(TAG,"Voltage Sensor Max Voltage (V): %f"       , Vmax_V );
    ESP_LOGI(TAG,"Voltage Sensor LSB (uV): %f"              , LSB_uV );
}

void init_adc_task(void *params){
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
    print_voltage_sensor_quantization();
    vTaskDelete(NULL);
}

void print_adc_data_task(void *params){

    while (true){
        // Get Data
        float Vref = get_5V_bus_voltage_V();
        float uA = get_voltage_sensor_bleed_current_uA();
        float mW = get_voltage_sensor_bleed_power_mW();

        // Print
        ESP_LOGI(TAG,"Voltage Sensor Value (V): %f", Vref);
        ESP_LOGI(TAG,"Voltage Sensor Bleed Current (uA): %f", uA);
        ESP_LOGI(TAG,"Voltage Sensor Bleed Power (mW): %f\n", mW);

        // Delay
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}


