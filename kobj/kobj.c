#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static struct attribute attr1 = {
	.name = "attr1",
	.mode = S_IRUGO,
};

static struct attribute attr2 = {
	.name = "attr2",
	.mode = S_IWUGO,
};

static struct attribute *my_attrs[] = {
	&attr1,
	&attr2,
	NULL
};

static ssize_t my_show(struct kobject *kobj, struct attribute *attr,
	char *buffer)
{
	strcpy(buffer, attr->name);
	pr_err("my show\n");
	return strlen(attr->name);
}

static ssize_t my_store(struct kobject *kobj, struct attribute *attr,
	const char *buffer, size_t size)
{
	pr_err("my store: %s\n", buffer);
	return size;
}

static const struct sysfs_ops my_ops = {
	.show = my_show,
	.store = my_store
};

static void my_release(struct kobject *kobj)
{
	pr_alert("my release\n");
}

static struct kobj_type my_type = {
	.release = my_release,
	.sysfs_ops = &my_ops,
	.default_attrs = my_attrs,
};

static struct kobject my_obj;

static int __init kobj_init(void)
{
	int ret;

	pr_alert("kobj init\n");

	memset(&my_obj, 0, sizeof(my_obj));
	kobject_init(&my_obj, &my_type);

	ret = kobject_add(&my_obj, NULL, "my_obj");
	if (ret) {
		pr_err("error initializing object\n");
		return 1;
	}

	return 0;
}

static void __exit kobj_exit(void)
{
	kobject_put(&my_obj);
	pr_alert("kobj exit\n");
}

module_init(kobj_init);
module_exit(kobj_exit);
