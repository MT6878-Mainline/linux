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

static const struct mtk_gate_regs im_c_s_cg_regs = {
	.set_ofs = 0xE08,
	.clr_ofs = 0xE04,
	.sta_ofs = 0xE00,
};

#define GATE_IM_CS(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &im_c_s_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate imp_cs_clks[] = {
	GATE_IM_CS(CLK_IM_C_S_I3C5_W1S, "im_c_s_i3c5_w1s",
			"i2c_ck"/* parent */, 0),
	GATE_IM_CS(CLK_IM_C_S_SEC_EN_W1S, "im_c_s_sec_w1s",
			"i2c_ck"/* parent */, 1),
};

static const struct mtk_clk_desc imp_iic_wrap_cs_desc = {
	.clks = imp_cs_clks,
	.num_clks = ARRAY_SIZE(imp_cs_clks),
};

static const struct mtk_gate_regs imp_es_cg_regs = {
	.set_ofs = 0xE08,
	.clr_ofs = 0xE04,
	.sta_ofs = 0xE00,
};

#define GATE_IMP_ESS(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &imp_es_cg_regs,		\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate imp_ess_clks[] = {
	GATE_IMP_ESS(CLK_IMP_ES_S_I3C10_W1S, "imp_es_s_i3c10_w1s",
			"i2c_ck"/* parent */, 0),
	GATE_IMP_ESS(CLK_IMP_ES_S_I3C11_W1S, "imp_es_s_i3c11_w1s",
			"i2c_ck"/* parent */, 1),
	GATE_IMP_ESS(CLK_IMP_ES_S_I3C12_W1S, "imp_es_s_i3c12_w1s",
			"i2c_ck"/* parent */, 2),
	GATE_IMP_ESS(CLK_IMP_ES_S_SEC_EN_W1S, "imp_es_s_sec_w1s",
			"i2c_ck"/* parent */, 3),
};

static const struct mtk_clk_desc imp_iic_wrap_ess_desc = {
	.clks = imp_ess_clks,
	.num_clks = CLK_IMP_ES_S_NR_CLK,
};

#define GATE_IMP_ES(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &imp_es_cg_regs,		\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate imp_es_clks[] = {
	GATE_IMP_ES(CLK_IMP_E_S_I3C0_W1S, "imp_e_s_i3c0_w1s",
			"i2c_ck"/* parent */, 0),
	GATE_IMP_ES(CLK_IMP_E_S_I3C1_W1S, "imp_e_s_i3c1_w1s",
			"i2c_ck"/* parent */, 1),
	GATE_IMP_ES(CLK_IMP_E_S_I3C2_W1S, "imp_e_s_i3c2_w1s",
			"i2c_ck"/* parent */, 2),
	GATE_IMP_ES(CLK_IMP_E_S_I3C4_W1S, "imp_e_s_i3c4_w1s",
			"i2c_ck"/* parent */, 3),
	GATE_IMP_ES(CLK_IMP_E_S_I3C9_W1S, "imp_e_s_i3c9_w1s",
			"i2c_ck"/* parent */, 4),
	GATE_IMP_ES(CLK_IMP_E_S_SEC_EN_W1S, "imp_e_s_sec_w1s",
			"i2c_ck"/* parent */, 5),
};

static const struct mtk_clk_desc imp_iic_wrap_es_desc = {
	.clks = imp_es_clks,
	.num_clks = ARRAY_SIZE(imp_es_clks),
};

static const struct mtk_gate_regs imp_ws_cg_regs = {
	.set_ofs = 0xE08,
	.clr_ofs = 0xE04,
	.sta_ofs = 0xE00,
};

#define GATE_IMP_WS(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &imp_ws_cg_regs,		\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate imp_w_s_clks[] = {
	GATE_IMP_WS(CLK_IMP_W_S_I3C3_W1S, "imp_w_s_i3c3_w1s",
			"i2c_ck"/* parent */, 0),
	GATE_IMP_WS(CLK_IMP_W_S_I3C6_W1S, "imp_w_s_i3c6_w1s",
			"i2c_ck"/* parent */, 1),
	GATE_IMP_WS(CLK_IMP_W_S_I3C7_W1S, "imp_w_s_i3c7_w1s",
			"i2c_ck"/* parent */, 2),
	GATE_IMP_WS(CLK_IMP_W_S_I3C8_W1S, "imp_w_s_i3c8_w1s",
			"i2c_ck"/* parent */, 3),
	GATE_IMP_WS(CLK_IMP_W_S_SEC_EN_W1S, "imp_w_s_sec_w1s",
			"i2c_ck"/* parent */, 4),
};

static const struct mtk_clk_desc imp_iic_wrap_ws_desc = {
	.clks = imp_w_s_clks,
	.num_clks = ARRAY_SIZE(imp_w_s_clks),
};

static const struct of_device_id of_match_clk_mt6878_imp_iic_wrap[] = {
	{ .compatible = "mediatek,mt6878-imp_iic_wrap-cs", .data = &imp_iic_wrap_cs_desc },
	{ .compatible = "mediatek,mt6878-imp_iic_wrap-ess", .data = &imp_iic_wrap_ess_desc },
	{ .compatible = "mediatek,mt6878-imp_iic_wrap-es", .data = &imp_iic_wrap_es_desc },
	{ .compatible = "mediatek,mt6878-imp_iic_wrap-ws", .data = &imp_iic_wrap_ws_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6878_imp_iic_wrap);

static struct platform_driver clk_mt6878_imp_iic_wrap_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6878-imp_iic_wrap",
		.of_match_table = of_match_clk_mt6878_imp_iic_wrap,
	},
};

MODULE_DESCRIPTION("MediaTek MT8196 pericfg clock controller driver");
module_platform_driver(clk_mt6878_imp_iic_wrap_drv);
MODULE_LICENSE("GPL");
