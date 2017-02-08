#include <linux/init.h>
#include <linux/module.h>

/* for platform drivers */
#include <linux/platform_device.h>

MODULE_LICENSE("Dual BSD/GPL");

#define print(...)	printk(KERN_ALERT "myplatform_drv: " __VA_ARGS__)

static int myplatfm_drv_probe(struct platform_device *pdev)
{
	struct resource *res;

	print("probe\n");
	print("device name: %s\n", pdev->name);
	print("device id: %d\n", pdev->id);
	print("number of resources: %d\n", pdev->num_resources);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (unlikely(!res)) {
		print("unable to aquire resource\n");
		return -1;
	}
	print("[MEM] start: %pa - end: %pa\n", res->start, res->end);

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 1);
	if (unlikely(!res)) {
		print("unable to aquire resource\n");
		return -1;
	}
	print("[IRQ] start: %pa - end: %pa\n", res->start, res->end);
	
	return 0;
}

static int myplatfm_drv_remove(struct platform_device *pdev)
{
	print("remove\n");
	return 0;
}

static struct platform_driver myplatfm_drv = {
	.driver = {
		.name = "myplatfm_drv",
		.owner = THIS_MODULE,
	},
	.probe = myplatfm_drv_probe,
	.remove = myplatfm_drv_remove,
};

static int __init myplatfm_drv_init(void)
{
	print("init\n");
	platform_driver_register(&myplatfm_drv);
	return 0;
}

static void __exit myplatfm_drv_exit(void)
{
	print("exit\n");
	platform_driver_unregister(&myplatfm_drv);
}

module_init(myplatfm_drv_init);
module_exit(myplatfm_drv_exit);
