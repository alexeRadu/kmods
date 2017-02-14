#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static struct kobj_type myset_type = {
	.sysfs_ops = NULL,
	.release = NULL,
	.default_attrs = NULL,
};

static struct kset myset;

static int __init kset_module_init(void)
{
	struct kobject *obj = &myset.kobj;

	memset(obj, 0, sizeof(*obj));
	
	kobject_init(obj, &myset_type);
	kobject_set_name(obj, "radu");
	
	kset_register(&myset);

	printk(KERN_ALERT "Hi world\n");
	return 0;
}

static void __exit kset_module_exit(void)
{
	kset_unregister(&myset);
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(kset_module_init);
module_exit(kset_module_exit);
