// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2020 TQ Systems GmbH
 */

#include <common.h>
#include <dm.h>
#include <dm/device-internal.h>
#include <dm/lists.h>
#include <dm/uclass-internal.h>
#include <malloc.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm-generic/gpio.h>
#include <fsl_esdhc.h>
#include <mmc.h>
#include <asm/arch/imx8mm_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/boot_mode.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/arch/clock.h>
#include <spl.h>


#include "../common/tqc_bb.h"
#include "../common/tqc_eeprom.h"

DECLARE_GLOBAL_DATA_PTR;
int board_early_init_f(void)
{
	tqc_bb_board_early_init_f();

	return 0;
}

int dram_init(void)
{
	/* rom_pointer[1] contains the size of TEE occupies */
	if (rom_pointer[1])
		gd->ram_size = PHYS_SDRAM_SIZE - rom_pointer[1];
	else
		gd->ram_size = PHYS_SDRAM_SIZE;

	return 0;
}

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, bd_t *bd)
{
	return tqc_bb_ft_board_setup(blob, bd);
}
#endif

int board_init(void)
{
	return tqc_bb_board_init();
}

static const char *tqma8mmx_get_boardname(void)
{
	switch (get_cpu_type()) {
	case MXC_CPU_IMX8MM:
		return "TQMa8MQML";
		break;
	case MXC_CPU_IMX8MML:
		return "TQMa8MQMLL";
		break;
	case MXC_CPU_IMX8MMD:
		return "TQMa8MDML";
		break;
	case MXC_CPU_IMX8MMDL:
		return "TQMa8MDMLL";
		break;
	case MXC_CPU_IMX8MMS:
		return "TQMa8MSML";
		break;
	case MXC_CPU_IMX8MMSL:
		return "TQMa8MSMLL";
		break;
	default:
		return "??";
	};
	return "UNKNOWN";
}

int print_bootinfo(void)
{
	enum boot_device bt_dev;
	bt_dev = get_boot_device();

	puts("Boot:  ");
	switch (bt_dev) {
	case SD1_BOOT:
		puts("SD0\n");
		break;
	case SD2_BOOT:
		puts("SD1\n");
		break;
	case MMC1_BOOT:
		puts("MMC0\n");
		break;
	case MMC2_BOOT:
		puts("MMC1\n");
		break;
	case USB_BOOT:
		puts("USB\n");
		break;
	default:
		printf("Unknown/Unsupported device %u\n", bt_dev);
		break;
	}

	return 0;
}

int board_late_init(void)
{
#if !defined(CONFIG_SPL_BUILD)
	struct tqc_eeprom_data eeprom;
	const char *bname = tqma8mmx_get_boardname();

	if (!tqc_read_eeprom_at(0, 0x53, 1, 0, &eeprom))
		tqc_board_handle_eeprom_data(bname, &eeprom);
	else
		puts("EEPROM: read error\n");
#endif

#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	env_set("board_name", tqc_bb_get_boardname());
	env_set("board_rev", tqma8mmx_get_boardname());
#endif

	return tqc_bb_board_late_init();
}

int checkboard(void)
{
	print_bootinfo();
	printf("Board: %s on a %s\n", tqma8mmx_get_boardname(),
	       tqc_bb_get_boardname());
	return 0;
}