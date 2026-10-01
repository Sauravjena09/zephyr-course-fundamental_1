#ifndef CUSTOM_DRIVERS_LED_SENSOR_H_
#define CUSTOM_DRIVERS_LED_SENSOR_H_

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int led_sensor_set_custom_param( const struct device *dev,int new_val);
#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_DRIVERS_LED_SENSOR_H_ */