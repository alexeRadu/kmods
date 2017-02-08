#include <linux/init.h>
#include <linux/module.h>

/* for platform device */
#include <linux/platform_device.h>

MODULE_LICENSE("Dual BSD/GPL");

#define print(...)	printk(KERN_ALERT "myplatfm_dev: " __VA_ARGS__)


static struct resource myplatfm_dev_resources[] = {
	{
		.start		= 0x00,
		.end 		= 0xFF,
		.flags		= IORESOURCE_MEM,
	},
	{
		.start 		= 10,
		.end 		= 10,
		.flags 		= IORESOURCE_IRQ,
	}
};

static struct platform_device myplatfm_dev = {
	.name		= "myplatfm_drv",
	.id 		= -1,
	.num_resources 	= ARRAY_SIZE(myplatfm_dev_resources),
	.resource 	= myplatfm_dev_resources,
};

static int __init platfm_dev_init(void)
{
	print("init\n");
	platform_device_register(&myplatfm_dev);
	return 0;
}

static void __exit platfm_dev_exit(void)
{
	platform_device_unregister(&myplatfm_dev);
	print("exit\n");
}

module_init(platfm_dev_init);
module_exit(platfm_dev_exit);
