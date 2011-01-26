#include <pspkernel.h>
#include <psputilsforkernel.h>
#include <pspsysevent.h>
#include <pspiofilemgr.h>
#include <stdio.h>
#include <string.h>
#include "systemctrl.h"
#include "utils.h"
#include "printk.h"

void sync_cache(void)
{
	sceKernelIcacheInvalidateAll();
	sceKernelDcacheWritebackInvalidateAll();
}

#ifdef DEBUG
char ownisgraph (u8 c)
{
    if ( c >= 0x21 && c <= 0x7e )
        return 1;

    return 0;
}

void hexdump(void *addr, int size)
{
	int i;
	u8 *p = (u8*)addr;

	if (addr == NULL) {
		printk("hexdump: <NULL>\n");

		return;
	}

	if (size == 0) {
		printk("hexdump: size 0\n");
		
		return;
	}

	printk("Address:   ");
	i=0; for(;i<16; ++i) {
		if (i == 8)
			printk("- ");
		
		printk("%02X ", i);
	}

	i=0; for(;i<16; ++i) {
		printk("%1X", i);
	}

	printk("\n-----------------------------------------------------------------------------\n");

	i=0;
	printk("0x%08X ", i);
	
	for(; i<size; ++i) {
		if (i != 0 && i % 16 == 0) {
			int j;

			for(j=16; j>0; --j) {
				if(ownisgraph(p[i-j])) {
					printk("%c", p[i-j]);
				} else {
					printk(".");
				}
			}
			printk("\n0x%08X ", i);
		}

		if (i != 0 && i % 8 == 0 && i % 16 != 0) {
			printk("- ");
		}

		printk("%02X ", p[i]);
	}

	int rest = (16-(i%16));

	rest = rest == 16 ? 0 : rest;
	int j; for(j=0; j<rest; j++) {
		if (j+(i%16) == 8)
			printk("  ");
		printk("   ");
	}

	rest = i % 16;
	rest = rest == 0 ? 16 : rest;

	for(j=rest; j>0; --j) {
		if(ownisgraph(p[i-j])) {
			printk("%c", p[i-j]);
		} else {
			printk(".");
		}
	}

	printk("\n");
}
#endif

int is_cpu_intr_enable(void)
{
	int ret;

	asm volatile ("mfic	%0, $0\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			: "=r"(ret)
			);

	return ret;
}

void fill_vram(u32 color)
{
	u32 *p = (u32*)0x44000000;

	while (p < (u32*)0x44200000) 
		*p++ = color;
}

int get_device_name(char *device, int size, const char* path)
{
	const char *p;

	if (path == NULL || device == NULL) {
		return -1;
	}

	p = strchr(path, '/');

	if (p == NULL) {
		return -2;
	}

	strncpy(device, path, MIN(size, p-path+1));
	device[MIN(size-1, p-path)] = '\0';

	printk("%s: device %s\n", __func__, device);

	return 0;
}
