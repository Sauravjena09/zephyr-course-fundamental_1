#include<zephyr/kernel.h>
#include<zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include<zephyr/logging/log.h>
#include<zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT custom_led_sensor
#warning ">>> COMPILER IS SUCCESSFULLY BUILDING LED_SENSOR.C <<<"
LOG_MODULE_REGISTER(led_sensor,LOG_LEVEL_INF);
struct led_sensor_data{
    uint8_t state;
};
struct led_sensor_config {
    struct gpio_dt_spec led_gpio;
};

static int led_sensor_sample_fetch(const struct device *dev ,enum sensor_channel chan){
    const struct led_sensor_config *cfg= dev->config;
    struct led_sensor_data *data=dev->data;
    int ret;
ret = gpio_pin_set_dt(&cfg->led_gpio, 1);
    if (ret < 0) {
        LOG_ERR("Failed to set LED ON: %d", ret);
        return ret;
    }

    data->state = 1;
    LOG_INF("LED Sensor: LED turned ON");
    return 0;
}
static int led_sensor_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val)
{
    const struct led_sensor_config *cfg = dev->config;
    struct led_sensor_data *data = dev->data;
    int ret;

    ret = gpio_pin_set_dt(&cfg->led_gpio, 0);
    if (ret < 0) {
        LOG_ERR("Failed to set LED OFF: %d", ret);
        return ret;
    }

    data->state = 0;
    LOG_INF("LED Sensor: LED turned OFF");

    /* Return state as a valid sensor_value reading */
    if (val != NULL) {
        val->val1 = 0;
        val->val2 = 0;
    }

    return 0;
}

static DEVICE_API(sensor, led_sensor_api) ={
    .sample_fetch = led_sensor_sample_fetch,
    .channel_get= led_sensor_channel_get,
};

static int led_sensor_init(const struct device *dev)
{
    const struct led_sensor_config *cfg = dev->config;
    struct led_sensor_data *data = dev->data;
    int ret;

    if (!gpio_is_ready_dt(&cfg->led_gpio)) {
        LOG_ERR("LED GPIO controller is not ready");
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&cfg->led_gpio, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure LED GPIO: %d", ret);
        return ret;
    }

    data->state = 0;
    LOG_INF("LED Sensor driver initialized");
    return 0;
}

/* 7. Instantiation macro per DT node */
#define LED_SENSOR_INIT(inst)                                                  \
    static struct led_sensor_data led_sensor_data_##inst;                      \
                                                                               \
    static const struct led_sensor_config led_sensor_config_##inst = {         \
        .led_gpio = GPIO_DT_SPEC_INST_GET(inst, led_gpios),                    \
    };                                                                         \
                                                                               \
    DEVICE_DT_INST_DEFINE(inst,                                                \
                          led_sensor_init,                                     \
                          NULL,                                                \
                          &led_sensor_data_##inst,                             \
                          &led_sensor_config_##inst,                           \
                          POST_KERNEL,                                         \
                          90,                         \
                          &led_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_INIT)