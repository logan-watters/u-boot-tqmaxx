/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * include/configs/tqmrzg2m_e-mbarzg2x.h
 *     This file is TQMaRZG2M-E (8GiB) module on MBaRZG2x board configuration.
 *
 * Copyright (C) 2019 Renesas Electronics Corporation
 * Copyright (C) 2020 TQ-Systems GmbH
 */

#ifndef __TQMARZG2M_E_MBARZG2X_H
#define __TQMARZG2M_E_MBARZG2X_H

#include "rcar-gen3-common.h"

/* Ethernet RAVB */
#define CONFIG_BITBANGMII
#define CONFIG_BITBANGMII_MULTI

/* Generic Timer Definitions (use in assembler source) */
#define COUNTER_FREQUENCY	0x17D7840	/* 25.00MHz from CPclk */

/* Environment in eMMC, at the end of 2nd "boot sector" */
#define CONFIG_ENV_OFFSET		(-CONFIG_ENV_SIZE)
#define CONFIG_SYS_MMC_ENV_DEV		1
#define CONFIG_SYS_MMC_ENV_PART		2

#define CONFIG_BOARD_LATE_INIT

#endif /*__TQMARZG2M_E_MBARZG2X_H */