#include "adc_drivers.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "ADC_DRIVERS"

/* Google "Voltage Divider Calculator" (V_out = V_ref*R2 / (R1 + R2))
*
*         V_ref (+5.5V)
*        ---------------                                      
*       |               |                          
*       |               \                          
*       |  R1(8.2kOhm)  /                           
*       |               \                          
*       |               |                          
*      ---              |-----------o
*       -               |                          
*       |               \               
*       |  R2(2.0kOhm)  /     V_out (1.1V)
*       |               \     
*       |               |
*        ---------------------------o
*/

void init_adc_task(void *params){
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
    vTaskDelete(NULL);
}

void print_adc_data_task(void *params){
    int val;
    float v_ref, v_out;

    while (true){
        val = adc1_get_raw(ADC1_CHANNEL_0);
        v_out = val/4095.0;                             // Convert to 1.1V. Using 12bit ADC.
        v_ref = v_out*(R1+R2)/R2;                       // Calculate 5V bus.
        ESP_LOGI(TAG,"ADC Raw Value: %d"    , val );
        ESP_LOGI(TAG,"ADC Vout Value: %f"   , v_out );
        ESP_LOGI(TAG,"ADC Vref Value: %f\n" , v_ref );

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}


