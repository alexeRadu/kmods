#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/spinlock.h>

MODULE_LICENSE("GPL");

#define SBULL_NAME		"sbull"
#define SBULL_MINORS		16
#define KERNEL_SECTOR_SIZE	512

static int sbull_major;
module_param(sbull_major, int, 0);
static int sector_size = 512;
module_param(sector_size, int, 0);
static int nsectors = 1024;
module_param(nsectors, int, 0);
static int ndevices = 4;
module_param(ndevices, int, 0);

struct sbull_dev {
	int size;
	u8 *data;
	spinlock_t lock;
	struct request_queue *queue;
	struct gendisk *gd;
};

static struct sbull_dev *devices;

static int sbull_open(struct block_device *bdev, fmode_t mode)
{
	struct gendisk *gd = bdev->bd_disk;

	printk("%s open\n", gd->disk_name);

	return 0;
}

static void sbull_release(struct gendisk *gd, fmode_t mode)
{
	printk("%s release\n", gd->disk_name);
}

static int sbull_ioctl(struct block_device *bdev, fmode_t mode,
			unsigned int cmd, unsigned long arg)
{
	printk("sbull ioctl\n");
	return 0;
}

static struct block_device_operations sbull_ops = {
	.owner = THIS_MODULE,
	.open = sbull_open,
	.release = sbull_release,
	.ioctl = sbull_ioctl
};

static void sbull_request(struct request_queue *q)
{
	struct request *req;

	printk ("processing requests\n");

	while ((req = blk_fetch_request(q)) != NULL) {
		//struct sbull_dev *dev = req->rq_disk->private_data;

		printk ("new request: sector: %lu, bytes %u\n", blk_rq_pos(req), blk_rq_bytes(req));

		if (req->cmd_type != REQ_TYPE_FS) {
			printk (KERN_NOTICE "skip non-fs requests\n");
			__blk_end_request_cur(req, -EIO);
			continue;
		}

		__blk_end_request_cur(req, 0);
	}
}

static void sbull_cleanup_module(void)
{
	int i;

	printk("sbull exit\n");

	if (devices) {
		for (i = 0; i < ndevices; i++) {
			if (devices[i].data)
				vfree(devices[i].data);

			if (devices[i].queue)
				blk_cleanup_queue(devices[i].queue);

			if (devices[i].gd)
				del_gendisk(devices[i].gd);
		}

		kfree(devices);
	}

	unregister_blkdev(sbull_major, SBULL_NAME);
}

static int __init sbull_init(void)
{
	int ret;
	int i;

	printk("sbull init\n");

	sbull_major = register_blkdev(sbull_major, SBULL_NAME);
	if (sbull_major < 0) {
		printk("unable to register %s device\n", SBULL_NAME);
		return sbull_major;
	}

	devices = kmalloc(ndevices * sizeof(struct sbull_dev), GFP_KERNEL);
	if (!devices) {
		ret = -ENOMEM;
		goto error;
	}

	memset(devices, 0, ndevices * sizeof(struct sbull_dev));

	for (i = 0; i < ndevices; i++) {
		printk("initialize device %d\n", i);

		devices[i].size = nsectors * sector_size;
		devices[i].data = vmalloc(devices[i].size);
		if (!devices[i].data) {
			printk(KERN_ERR "unable to allocate device %d error\n", i);
			ret = -ENOMEM;
			goto error;
		}

		spin_lock_init(&devices[i].lock);

		devices[i].queue = blk_init_queue(sbull_request, &devices[i].lock);
		if (!devices[i].queue) {
			printk(KERN_ERR "unable to allocate device queue\n");
			ret = -ENOMEM;
			goto error;
		}
		blk_queue_logical_block_size(devices[i].queue, sector_size);
		devices[i].queue->queuedata = &devices[i];

		devices[i].gd = alloc_disk(SBULL_MINORS);
		if (!devices[i].gd) {
			printk(KERN_ERR "unable to allocate gendisk\n");
			ret = -ENOMEM;
			goto error;
		}

		devices[i].gd->major = sbull_major;
		devices[i].gd->first_minor = i * SBULL_MINORS;
		devices[i].gd->fops = &sbull_ops;
		devices[i].gd->queue = devices[i].queue;
		devices[i].gd->private_data = &devices[i];
		snprintf(devices[i].gd->disk_name, 32, "sbull%c", 'a' + i);
		set_capacity(devices[i].gd, nsectors * (sector_size / KERNEL_SECTOR_SIZE));
		add_disk(devices[i].gd);
	}	

	return 0;

error:
	sbull_cleanup_module();
	return ret;
}

module_init(sbull_init);
module_exit(sbull_cleanup_module);
