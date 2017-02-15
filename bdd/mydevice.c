#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

MODULE_LICENSE("Dual BSD/GPL");

extern struct bus_type mybus_bus_type;

static void mydevice_release (struct device *dev)
{
	pr_info("mydevice release\n");
}

static struct device mydevice = {
	.init_name 	= "mydevice",
	.release 	= mydevice_release,
	.bus 		= &mybus_bus_type,
};

static int __init mydevice_init(void)
{
	int ret;

	ret = device_register(&mydevice);
	if (ret) {
		pr_err("unable to register device %s\n", mydevice.init_name);
		return 1;
	}

	pr_info("mydevice init done\n");
	return 0;
}

static void __exit mydevice_exit(void)
{
	device_unregister(&mydevice);
	pr_info("mydevice exit done\n");
}

module_init(mydevice_init);
module_exit(mydevice_exit);