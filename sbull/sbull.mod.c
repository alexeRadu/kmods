#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xc6c01fa, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x16abfc10, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0xf42bf609, __VMLINUX_SYMBOL_STR(add_disk) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xb493c9a4, __VMLINUX_SYMBOL_STR(alloc_disk) },
	{ 0xf8749efa, __VMLINUX_SYMBOL_STR(blk_queue_logical_block_size) },
	{ 0x9e3cd874, __VMLINUX_SYMBOL_STR(blk_init_queue) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x71a50dbc, __VMLINUX_SYMBOL_STR(register_blkdev) },
	{ 0xb5a459dc, __VMLINUX_SYMBOL_STR(unregister_blkdev) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x5e3ef3d6, __VMLINUX_SYMBOL_STR(del_gendisk) },
	{ 0x2a6bc1e3, __VMLINUX_SYMBOL_STR(blk_cleanup_queue) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x7c52a47d, __VMLINUX_SYMBOL_STR(__blk_end_request_cur) },
	{ 0xe703a84, __VMLINUX_SYMBOL_STR(blk_fetch_request) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "2D0E08C51705397C55725C8");
