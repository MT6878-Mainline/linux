// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2023 MediaTek Inc.
 * 		 Chuan-wen Chen <chuan-wen.chen@mediatek.com>
 * Copyright (c) 2025 Igor Belwon <igor.belwon@mentallysanemainliners.org>
 */

#include <dt-bindings/clock/mediatek,mt6878-clock.h>
#include <linux/clk-provider.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs ufsao_cg_regs = {
	.set_ofs = 0x8,
	.clr_ofs = 0xC,
	.sta_ofs = 0x4,
};

#define GATE_UFSAO(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &ufsao_cg_regs,			\
		.shift = _shift,			\
		.flags = CLK_OPS_PARENT_ENABLE,		\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate ufsao_clks[] = {
	GATE_UFSAO(CLK_UFSAO_UNIPRO_TX_SYM, "ufsao_unipro_tx_sym",
			"top_f26m_ck"/* parent */, 0),
	GATE_UFSAO(CLK_UFSAO_UNIPRO_RX_SYM0, "ufsao_unipro_rx_sym0",
			"top_f26m_ck"/* parent */, 1),
	GATE_UFSAO(CLK_UFSAO_UNIPRO_RX_SYM1, "ufsao_unipro_rx_sym1",
			"top_f26m_ck"/* parent */, 2),
	GATE_UFSAO(CLK_UFSAO_UNIPRO_SYS, "ufsao_unipro_sys",
			"top_ufs_ck"/* parent */, 3),
	GATE_UFSAO(CLK_UFSAO_UNIPRO_SAP_CFG, "ufsao_unipro_sap_cfg",
			"top_f26m_ck"/* parent */, 8),
	GATE_UFSAO(CLK_UFSAO_PHY_TOP_AHB_S_BUS, "ufsao_phy_ahb_s_bus",
			"hf_fufs_faxi_ck"/* parent */, 9),
};

static const struct mtk_clk_desc ufsao_mcd = {
	.clks = ufsao_clks,
	.num_clks = ARRAY_SIZE(ufsao_clks),
};

static const struct of_device_id of_match_clk_mt6878_ufs_ao[] = {
	{ .compatible = "mediatek,mt6878-ufscfg-ao", .data = &ufsao_mcd },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6878_ufs_ao);

static struct platform_driver clk_mt6878_ufs_ao_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6878-ufs-ao",
		.of_match_table = of_match_clk_mt6878_ufs_ao,
	},
};

module_platform_driver(clk_mt6878_ufs_ao_drv);
MODULE_DESCRIPTION("MediaTek MT6878 ufs_ao clocks driver");
MODULE_LICENSE("GPL");
