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

static const struct mtk_gate_regs ufspdn_cg_regs = {
	.set_ofs = 0x8,
	.clr_ofs = 0xC,
	.sta_ofs = 0x4,
};

#define GATE_UFSPDN(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &ufspdn_cg_regs,			\
		.shift = _shift,			\
		.flags = CLK_OPS_PARENT_ENABLE,		\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate ufspdn_clks[] = {
	GATE_UFSPDN(CLK_UFSPDN_UFSHCI, "ufspdn_ufshci",
			"top_ufs_ck"/* parent */, 0),
	GATE_UFSPDN(CLK_UFSPDN_UFSHCI_AES, "ufspdn_ufshci_aes",
			"top_aes_ufsfde_ck"/* parent */, 1),
	GATE_UFSPDN(CLK_UFSPDN_UFSHCI_AHB, "ufspdn_UFSHCI_ahb",
			"hf_fufs_faxi_ck"/* parent */, 3),
	GATE_UFSPDN(CLK_UFSPDN_UFSHCI_AXI, "ufspdn_UFSHCI_axi",
			"hf_fufs_fmem_sub_ck"/* parent */, 5),
};

static const struct mtk_clk_desc ufspdn_mcd = {
	.clks = ufspdn_clks,
	.num_clks = ARRAY_SIZE(ufspdn_clks),
};

static const struct of_device_id of_match_clk_mt6878_ufs_pdn[] = {
	{ .compatible = "mediatek,mt6878-ufscfg-pdn", .data = &ufspdn_mcd },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6878_ufs_pdn);

static struct platform_driver clk_mt6878_ufs_pdn_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6878-ufs-pdn",
		.of_match_table = of_match_clk_mt6878_ufs_pdn,
	},
};

module_platform_driver(clk_mt6878_ufs_pdn_drv);
MODULE_DESCRIPTION("MediaTek MT6878 ufs_pdn clocks driver");
MODULE_LICENSE("GPL");
