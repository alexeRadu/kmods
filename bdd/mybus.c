#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

MODULE_LICENSE("Dual BSD/GPL");

#define MY_BUS_VERSION_MAJOR		0
#define MY_BUS_VERSION_MINOR		1		


ssize_t my_bus_vers_show (struct bus_type *bus, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d.%d\n", MY_BUS_VERSION_MAJOR,
		MY_BUS_VERSION_MINOR);
}

static BUS_ATTR(version, S_IRUGO, my_bus_vers_show, NULL);

static int mybus_match(struct device *dev, struct device_driver *driver)
{
	pr_info("mybus_match called\n");
	return 1;
}

struct bus_type mybus_bus_type = {
	.name 		= "mybus",
	.match 		= mybus_match,
};
EXPORT_SYMBOL_GPL(mybus_bus_type);

static int __init mybus_init(void)
{
	int ret;

	ret = bus_register(&mybus_bus_type);
	if (ret) {
		pr_err("Unable to register bus %s\n", mybus_bus_type.name);
		return 1;
	}

	ret = bus_create_file(&mybus_bus_type, &bus_attr_version);
	if (ret) {
		pr_err("Unable to create attribute version\n");
	}

	pr_info("mybus init done\n");
	return 0;
}

static void __exit mybus_exit(void)
{
	bus_unregister(&mybus_bus_type);
	pr_info("mybus exit done\n");
}

module_init(mybus_init);
module_exit(mybus_exit);
