#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

MODULE_LICENSE("Dual BSD/GPL");

extern struct bus_type mybus_bus_type;

static int mydriver_probe(struct device *dev)
{
	pr_info("mydriver probe for device %s\n", dev->init_name);
	return 0;
}

static int mydriver_remove(struct device *dev)
{
	pr_info("mydriver remove for device %s\n", dev->init_name);
	return 0;
}

static void mydriver_shutdown(struct device *dev)
{
	pr_info("mydriver shutdown\n");
}


static struct device_driver mydriver = {
	.name 	= "mydriver",
	.bus 	= &mybus_bus_type,
	.probe 	= mydriver_probe,
	.remove = mydriver_remove,
	.shutdown = mydriver_shutdown,
};

static int __init mydriver_init(void)
{
	int ret;

	ret = driver_register(&mydriver);
	if (ret) {
		pr_err("Error registering driver %s\n", mydriver.name);
		return 1;
	}

	pr_info("mydriver init done\n");
	return 0;
}

static void __exit mydriver_exit(void)
{
	driver_unregister(&mydriver);
	pr_info("mydriver exit done\n");
}

module_init(mydriver_init);
module_exit(mydriver_exit);