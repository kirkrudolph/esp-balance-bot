# Balance Bot

Goal: Balance on two wheels.
Stretch Goal: Balance on one wheel.

## [Detecting ADC Calibration](https://docs.espressif.com/projects/esp-idf/en/release-v4.3/esp32/api-reference/peripherals/adc.html#calibration-values)
`$IDF_PATH/components/esptool_py/esptool/espefuse.py --port /dev/cu.usbserial-0001 adc_info`

```
Connecting........_____.....___
Detecting chip type... ESP32
espefuse.py v3.1-dev
ADC VRef calibration: 1100mV
```