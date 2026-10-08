#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <stdlib.h>
#include "custom_drivers/led_sensor.h"

LOG_MODULE_REGISTER(main);

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv) {
    const struct device *dev = DEVICE_DT_GET_ANY(custom_led_sensor);
    if (dev == NULL || !device_is_ready(dev)) {
        shell_error(sh, "Device not ready");
        return -ENODEV;
    }
    sensor_sample_fetch(dev);
    shell_print(sh, "sensor_sample_fetch() called");
    return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv) {
    const struct device *dev = DEVICE_DT_GET_ANY(custom_led_sensor);
    if (dev == NULL || !device_is_ready(dev)) {
        shell_error(sh, "Device not ready");
        return -ENODEV;
    }
    struct sensor_value val;
    sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
    shell_print(sh, "sensor_channel_get() called. Result: val1=%d, val2=%d", val.val1, val.val2);
    return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv) {
    const struct device *dev = DEVICE_DT_GET_ANY(custom_led_sensor);
    if (dev == NULL) {
        shell_error(sh, "No device found");
        return -ENODEV;
    }
    shell_print(sh, "Device name: %s", dev->name);
    shell_print(sh, "Ready state: %s", device_is_ready(dev) ? "true" : "false");
    return 0;
}

static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv) {
    const struct device *dev = DEVICE_DT_GET_ANY(custom_led_sensor);
    if (dev == NULL || !device_is_ready(dev)) {
        shell_error(sh, "Device not ready");
        return -ENODEV;
    }

    if (argc < 2) {
        shell_error(sh, "Missing argument");
        return -EINVAL;
    }

    long val = strtol(argv[1], NULL, 10);
    if (val < 0 || val > 100) {
        shell_error(sh, "Argument out of range (must be 0-100)");
        return -EINVAL;
    }

    int ret = led_sensor_set_custom_param(dev, (int)val);
    if (ret == 0) {
        shell_print(sh, "Custom param set to %d", (int)val);
    } else {
        shell_error(sh, "Failed to set custom param");
    }
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "calls sensor_sample_fetch()", cmd_sensor_fetch),
    SHELL_CMD(info, NULL, "prints the device name and ready state", cmd_sensor_info),
    SHELL_CMD(read, NULL, "calls sensor_channel_get() and prints the result", cmd_sensor_read),
    SHELL_CMD_ARG(set, NULL, "sets a custom value", cmd_sensor_set, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Sensor shell command", NULL);

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
    LOG_INF("Shell is enabled. Try running 'sensor info', 'sensor fetch', 'sensor read', or 'sensor set <val>' over UART.");

    return 0;
}