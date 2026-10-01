#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

int main(void)
{
    /* Fetch the device handle mapped to our compatible */
    const struct device *dev = DEVICE_DT_GET_ANY(custom_led_sensor);

    if (dev == NULL) {
        LOG_ERR("No device found with compatible 'custom,led-sensor'");
        return -ENODEV;
    }

    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return -ENODEV;
    }

    LOG_INF("Found ready device: %s", dev->name);

    while (1) {
        /* Task 1: sensor_sample_fetch turns the LED ON */
        LOG_INF("Calling sensor_sample_fetch (turning LED ON)...");
        sensor_sample_fetch(dev);
        k_sleep(K_MSEC(1000));

        /* Task 1: sensor_channel_get turns the LED OFF */
        LOG_INF("Calling sensor_channel_get (turning LED OFF)...");
        sensor_channel_get(dev, SENSOR_CHAN_ALL, NULL);
        k_sleep(K_MSEC(1000));
    }

    return 0;
}