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

## Thanks to the following articles
- [L298n Noise](https://forum.arduino.cc/t/l298n-noises/260901)
- [L298n Driver](https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/#google_vignette)

## Should I include these?
- [Extra Mount](https://www.thingiverse.com/thing:26538)


## IMU Options

### MPU6050 (InvenSense Legacy)
- [Datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
- [Buy](https://www.aliexpress.com/item/2251832854058984.html?spm=a2g0o.search0304.0.0.36e828a0NaUhI2&algo_pvid=53de270e-d6ec-4cbd-8aef-9cc94f50a072&aem_p4p_detail=2022072323185813735359496113040010644831&algo_exp_id=53de270e-d6ec-4cbd-8aef-9cc94f50a072-3&pdp_ext_f=%7B%22sku_id%22%3A%2267317139951%22%7D&pdp_npi=2%40dis%21USD%21%2112.99%21%21%21%21%21%402101fd4b16586435387782449e5da2%2167317139951%21sea)

### ICM-42605 (TDK New Designs)
- [Datasheet](https://www.cdiweb.com/datasheets/invensense/ds-icm-42605v1-2.pdf)
- [Buy](https://www.cdiweb.com/products/detail/icm42605-tdk-invensense/637289/?utm_source=netcomponents&utm_medium=inventory-feed&utm_campaign=buy-now-netcomponents&pid=2265)
