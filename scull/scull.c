#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

#define SCULL_MAJOR_NUM		0
#define SCULL_DEV_COUNT		4
#define SCULL_DEV_FIRST_MINOR	0
#define SCULL_DEV_NAME		"scull"

int scull_major_num = SCULL_MAJOR_NUM;

struct scull_dev {
	int minor_num;
	struct cdev cdev;
};

static struct scull_dev *scull_devices;

int scull_open(struct inode *inode, struct file *filp)
{
	struct scull_dev *dev;

	printk("scull open for minor %d\n", iminor(inode));

	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev;
	
	return 0;
}

int scull_release(struct inode *inode, struct file *filp)
{
	printk("scull release for minor %d\n", iminor(inode));

	return 0;
}

ssize_t scull_read(struct file *filp, char __user *buf, size_t count,
		loff_t *f_pos)
{
	printk("scull read minor %d\n", iminor(filp->f_inode));
	return count;
}

ssize_t scull_write(struct file *filp, const char __user *buf, size_t count,
		loff_t *f_pos)
{
	printk("scull write minor %d\n", iminor(filp->f_inode));
	return count;
}

long scull_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("scull ioctl\n");
	return 0;
}

loff_t scull_llseek(struct file *filp, loff_t off, int whence)
{
	printk("scull llseek\n");
	return 0;
}

struct file_operations scull_fops = {
	.owner = 	THIS_MODULE,
	.llseek = 	scull_llseek,
	.read = 	scull_read,
	.write = 	scull_write,
	.unlocked_ioctl = scull_ioctl,
	.open = 	scull_open,
	.release = 	scull_release,
};

static void scull_cleanup_module(void)
{
	int i;
	dev_t devno = MKDEV(scull_major_num, SCULL_DEV_FIRST_MINOR);

	printk("scull exit\n");

	if (scull_devices) {
		for (i = 0; i < SCULL_DEV_COUNT; i++)
			cdev_del(&scull_devices->cdev);

		kfree(scull_devices);
	}

	unregister_chrdev_region(devno, SCULL_DEV_COUNT);
}

static int scull_setup_cdev(struct scull_dev *dev, int index)
{
	int ret;
	dev_t devno;

	devno = MKDEV(scull_major_num, SCULL_DEV_FIRST_MINOR + index);

	cdev_init(&dev->cdev, &scull_fops);

	dev->cdev.owner = THIS_MODULE;
	ret = cdev_add(&dev->cdev, devno, 1);
	if (ret)
		printk(KERN_ERR "Error adding scull\n");
	
	return ret;
}

static int __init scull_init(void)
{
	dev_t dev_num;
	int i;
	int ret;

	printk("scull init\n");

	if (scull_major_num) {
		dev_num = MKDEV(scull_major_num, SCULL_DEV_FIRST_MINOR);
		ret = register_chrdev_region(dev_num, SCULL_DEV_COUNT, SCULL_DEV_NAME);

	} else {
		ret = alloc_chrdev_region(&dev_num, SCULL_DEV_FIRST_MINOR, SCULL_DEV_COUNT, "scull");
		scull_major_num = MAJOR(dev_num);
	}

	if (ret) {
		printk(KERN_ERR "unable to register scull device number\n");
		return ret;
	}

	scull_devices = kmalloc(SCULL_DEV_COUNT * sizeof(struct scull_dev), GFP_KERNEL);
	if (!scull_devices) {
		ret = -ENOMEM;
		goto fail;
	}

	for (i = 0; i < SCULL_DEV_COUNT; i++) {
		scull_devices[i].minor_num = SCULL_DEV_COUNT + i;
		ret = scull_setup_cdev(&scull_devices[i], i);
		if (ret)
			goto fail;
	}	

	return ret;

fail:
	scull_cleanup_module();
	return ret;
}

module_init(scull_init);
module_exit(scull_cleanup_module);
