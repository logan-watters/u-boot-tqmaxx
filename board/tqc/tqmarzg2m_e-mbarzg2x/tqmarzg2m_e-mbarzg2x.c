// SPDX-License-Identifier: GPL-2.0+
/*
 * board/tqc/tqmarzg2m_e-mbarzg2x/tqmarzg2m_e-mbarzg2x.c
 *     This file provides TQMaRZG2M board support.
 *
 * Copyright (C) 2015-2019 Renesas Electronics Corporation
 * Copyright (C) 2015 Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
 * Copyright (C) 2020 TQ-Systems GmbH
 */

#include <common.h>
#include <malloc.h>
#include <netdev.h>
#include <dm.h>
#include <dm/platform_data/serial_sh.h>
#include <asm/processor.h>
#include <asm/mach-types.h>
#include <asm/io.h>
#include <linux/errno.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <asm/arch/gpio.h>
#include <asm/arch/rmobile.h>
#include <asm/arch/rcar-mstp.h>
#include <asm/arch/sh_sdhi.h>
#include <i2c.h>
#include <mmc.h>

DECLARE_GLOBAL_DATA_PTR;

static int board_rev;

void s_init(void)
{
}

#define GPIO_WLAN_REG_ON 157
#define GPIO_BT_REG_ON 158
#define	GPIO_BT_POWER		73	/* GP3_13	*/
#define	GPIO_WIFI_POWER		82	/* GP4_06	*/

void clear_wlan_bt_reg_on(void)
{
	if (board_rev > 2) {
		gpio_request(GPIO_BT_POWER, "bt_power");
		gpio_request(GPIO_WIFI_POWER, "wifi_power");
		gpio_direction_output(GPIO_BT_POWER, 0);
		gpio_direction_output(GPIO_WIFI_POWER, 0);
	} else {
		gpio_request(GPIO_WLAN_REG_ON, "wlan_reg_on");
		gpio_request(GPIO_BT_REG_ON, "bt_reg_on");
		gpio_direction_output(GPIO_WLAN_REG_ON, 0);
		gpio_direction_output(GPIO_BT_REG_ON, 0);
	}
}

#define SCIF2_MSTP310		BIT(10)	/* SCIF2 */
#define DVFS_MSTP926		BIT(26)
#define GPIO2_MSTP910		BIT(10)
#define GPIO3_MSTP909		BIT(9)
#define GPIO5_MSTP907		BIT(7)
#define HSUSB_MSTP704		BIT(4)	/* HSUSB */

int board_early_init_f(void)
{
#if defined(CONFIG_SYS_I2C) && defined(CONFIG_SYS_I2C_SH)
	/* DVFS for reset */
	mstp_clrbits_le32(SMSTPCR9, SMSTPCR9, DVFS_MSTP926);
#endif
	return 0;
}

/* HSUSB block registers */
#define HSUSB_REG_LPSTS			0xE6590102
#define HSUSB_REG_LPSTS_SUSPM_NORMAL	BIT(14)
#define HSUSB_REG_UGCTRL2		0xE6590184
#define HSUSB_REG_UGCTRL2_USB0SEL	0x30
#define HSUSB_REG_UGCTRL2_USB0SEL_EHCI	0x10

#define	GPIO_REV_BIT1		113	/* GP5_19	*/
#define	GPIO_REV_BIT0		115	/* GP5_21	*/

#define CLOCKGEN_I2C_BUS_NUM	4
#define CLOCKGEN_I2C_ADDR	0x6A

static int clockgen_init(void)
{
	struct udevice dev;
	struct udevice *pdev = &dev;
	int ret;

	ret = i2c_get_chip_for_busnum(CLOCKGEN_I2C_BUS_NUM, CLOCKGEN_I2C_ADDR, 1, &pdev);

	if (ret)
		return ret;

	u8 regvals[][2] = {
		/* Start configuration preamble */
		/*    Set device in Ready mode */
		{ 0x06, 0x01 },
		/* End configuration preamble */

		/* Start configuration registers */
		{ 0x17, 0x00 },
		{ 0x18, 0x00 },
		{ 0x19, 0x00 },
		{ 0x1A, 0x00 },
		{ 0x1B, 0x00 },
		{ 0x1C, 0x00 },
		{ 0x21, 0x34 },
		{ 0x24, 0x01 },
		{ 0x25, 0x10 },
		{ 0x26, 0x00 },
		{ 0x27, 0x00 },
		{ 0x28, 0x60 },
		{ 0x2A, 0x50 },
		{ 0x2B, 0x19 },
		{ 0x2D, 0x14 },
		{ 0x36, 0x32 },
		{ 0x37, 0xDC },
		{ 0x38, 0x02 },
		{ 0x39, 0x80 },
		{ 0x3A, 0x03 },
		{ 0x3B, 0x00 },
		{ 0x3C, 0x00 },
		{ 0x48, 0x00 },
		{ 0x4E, 0x25 },
		{ 0x4F, 0xE0 },
		{ 0x50, 0x00 },
		{ 0x51, 0x20 },
		{ 0x52, 0x00 },
		{ 0x53, 0x21 },
		{ 0x54, 0x00 },
		{ 0x60, 0x00 },
		{ 0x67, 0x19 },
		{ 0x68, 0x00 },
		{ 0x69, 0x00 },
		{ 0x6A, 0x00 },
		{ 0x6B, 0x00 },
		{ 0x6C, 0x01 },
		{ 0x73, 0x00 },
		{ 0x74, 0x00 },
		{ 0x75, 0x01 },
		{ 0x7A, 0x01 },
		{ 0x7B, 0x01 },
		{ 0x7C, 0x00 },
		{ 0x7D, 0x00 },
		{ 0x7E, 0x00 },
		{ 0x7F, 0x09 },
		{ 0x80, 0x01 },
		{ 0x81, 0x00 },
		{ 0x82, 0x00 },
		{ 0x84, 0x09 },
		{ 0x85, 0x01 },
		{ 0x86, 0x00 },
		{ 0x87, 0x00 },
		{ 0x89, 0x09 },
		{ 0x8A, 0x01 },
		{ 0x8B, 0x00 },
		{ 0x8C, 0x00 },
		{ 0x8E, 0x09 },
		{ 0x8F, 0x01 },
		{ 0x90, 0x00 },
		{ 0x91, 0x00 },
		{ 0x93, 0x09 },
		{ 0x94, 0x01 },
		{ 0x95, 0x00 },
		{ 0x96, 0x00 },
		{ 0x98, 0x01 },
		{ 0x99, 0x01 },
		{ 0x9A, 0x00 },
		{ 0x9B, 0x00 },
		{ 0x9C, 0x00 },
		{ 0x9D, 0x01 },
		{ 0x9E, 0x01 },
		{ 0x9F, 0x00 },
		{ 0xA0, 0x00 },
		{ 0xA1, 0x00 },
		{ 0xA2, 0x01 },
		{ 0xA3, 0x01 },
		{ 0xA4, 0x00 },
		{ 0xA5, 0x00 },
		{ 0xA6, 0x00 },
		{ 0xA7, 0x07 },
		{ 0xA9, 0x00 },
		{ 0xAA, 0x00 },
		{ 0xAC, 0x01 },
		{ 0xAD, 0x02 },
		{ 0xAE, 0x00 },
		{ 0xAF, 0x00 },
		{ 0xB0, 0x00 },
		{ 0xB1, 0x01 },
		{ 0xB2, 0x01 },
		{ 0xB3, 0x00 },
		{ 0xB4, 0x00 },
		{ 0xB5, 0x00 },
		{ 0xB6, 0xDF },
		{ 0xB7, 0x0D },
		{ 0xB9, 0x06 },
		{ 0xBA, 0x1C },
		{ 0xBB, 0x10 },
		{ 0xBC, 0x00 },
		{ 0xBD, 0x02 },
		{ 0xBE, 0x20 },
		/* End configuration registers */

		/* Start configuration postamble */
		/*    Set device in Active mode */
		{ 0x06, 0x02 }
		/* End configuration postamble */
	};

	for (int i = 0; i < ARRAY_SIZE(regvals); i++) {
		ret = dm_i2c_write(pdev, regvals[i][0], &regvals[i][1], 1);
		if (ret) {
			printf("Error writing to clock generator!\n");
			return 0;
		}
	}

	return 0;
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_TEXT_BASE + 0x50000;

	/* USB1 pull-up */
	setbits_le32(PFC_PUEN6, PUEN_USB1_OVC | PUEN_USB1_PWEN);

	/* Configure the HSUSB block */
	mstp_clrbits_le32(SMSTPCR7, SMSTPCR7, HSUSB_MSTP704);
	/* Choice USB0SEL */
	clrsetbits_le32(HSUSB_REG_UGCTRL2, HSUSB_REG_UGCTRL2_USB0SEL,
			HSUSB_REG_UGCTRL2_USB0SEL_EHCI);
	/* low power status */
	setbits_le16(HSUSB_REG_LPSTS, HSUSB_REG_LPSTS_SUSPM_NORMAL);

	clockgen_init();

	if ((rmobile_get_cpu_rev_integer() == 1) && (rmobile_get_cpu_rev_fraction() < 3)) {
		board_rev = 2;
	} else {
		gpio_request(GPIO_REV_BIT0, "rev_bit0");
		gpio_request(GPIO_REV_BIT1, "rev_bit1");
		gpio_direction_input(GPIO_REV_BIT1);
		gpio_direction_input(GPIO_REV_BIT0);
		board_rev = 0x03 + ((gpio_get_value(GPIO_REV_BIT1) << 1)  | gpio_get_value(GPIO_REV_BIT0));
	}
	clear_wlan_bt_reg_on();

	return 0;
}

int board_late_init(void)
{
	env_set_hex("board_rev", board_rev);

	return 0;
}

int dram_init(void)
{
	if (fdtdec_setup_mem_size_base() != 0)
		return -EINVAL;

	return 0;
}

int dram_init_banksize(void)
{
	fdtdec_setup_memory_banksize();

	return 0;
}

#define RST_BASE	0xE6160000
#define RST_CA57RESCNT	(RST_BASE + 0x40)
#define RST_CA53RESCNT	(RST_BASE + 0x44)
#define RST_RSTOUTCR	(RST_BASE + 0x58)
#define RST_CODE	0xA5A5000F

void reset_cpu(ulong addr)
{
#if defined(CONFIG_SYS_I2C) && defined(CONFIG_SYS_I2C_SH)
	i2c_reg_write(CONFIG_SYS_I2C_POWERIC_ADDR, 0x20, 0x80);
#else
	/* only CA57 ? */
	writel(RST_CODE, RST_CA57RESCNT);
#endif
}

void board_add_ram_info(int use_default)
{
	int i;

	printf("\n");
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		if (!gd->bd->bi_dram[i].size)
			break;
		printf("Bank #%d: 0x%09llx - 0x%09llx, ", i,
		       (unsigned long long)(gd->bd->bi_dram[i].start),
		       (unsigned long long)(gd->bd->bi_dram[i].start
		       + gd->bd->bi_dram[i].size - 1));
		print_size(gd->bd->bi_dram[i].size, "\n");
	};
}

void board_cleanup_before_linux(void)
{
	/*
	 * Turn off the clock that was turned on outside
	 * the control of the driver
	 */
	/* Configure the HSUSB block */
	mstp_setbits_le32(SMSTPCR7, SMSTPCR7, HSUSB_MSTP704);

	/*
	 * Because of the control order dependency,
	 * turn off a specific clock at this timing
	 */
	mstp_setbits_le32(SMSTPCR9, SMSTPCR9,
			  GPIO2_MSTP910 | GPIO3_MSTP909 | GPIO5_MSTP907);
}