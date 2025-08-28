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

static const struct mtk_gate_regs peri_ao0_cg_regs = {
	.set_ofs = 0x24,
	.clr_ofs = 0x28,
	.sta_ofs = 0x10,
};

static const struct mtk_gate_regs peri_ao1_cg_regs = {
	.set_ofs = 0x2c,
	.clr_ofs = 0x30,
	.sta_ofs = 0x14,
};

static const struct mtk_gate_regs peri_ao2_cg_regs = {
	.set_ofs = 0x34,
	.clr_ofs = 0x38,
	.sta_ofs = 0x18,
};

#define GATE_PERI_AO0(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &peri_ao0_cg_regs,		\
		.shift = _shift,			\
		.flags = CLK_OPS_PARENT_ENABLE,		\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

#define GATE_PERI_AO1(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &peri_ao1_cg_regs,		\
		.shift = _shift,			\
		.flags = CLK_OPS_PARENT_ENABLE,		\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

#define GATE_PERI_AO2(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &peri_ao2_cg_regs,		\
		.shift = _shift,			\
		.flags = CLK_OPS_PARENT_ENABLE,		\
		.ops = &mtk_clk_gate_ops_setclr,	\
	}

static const struct mtk_gate peri_ao_clks[] = {
	/* PERAO0 */
	GATE_PERI_AO0(CLK_PERAOP_UART0, "peraop_uart0",
			"top_uart_ck"/* parent */, 0),
	GATE_PERI_AO0(CLK_PERAOP_UART1, "peraop_uart1",
			"top_uart_ck"/* parent */, 1),
	GATE_PERI_AO0(CLK_PERAOP_UART2, "peraop_uart2",
			"top_uart_ck"/* parent */, 2),
	GATE_PERI_AO0(CLK_PERAOP_PWM_H, "peraop_pwm_h",
			"top_axip_ck"/* parent */, 4),
	GATE_PERI_AO0(CLK_PERAOP_PWM_B, "peraop_pwm_b",
			"top_pwm_ck"/* parent */, 5),
	GATE_PERI_AO0(CLK_PERAOP_PWM_FB1, "peraop_pwm_fb1",
			"top_pwm_ck"/* parent */, 6),
	GATE_PERI_AO0(CLK_PERAOP_PWM_FB2, "peraop_pwm_fb2",
			"top_pwm_ck"/* parent */, 7),
	GATE_PERI_AO0(CLK_PERAOP_PWM_FB3, "peraop_pwm_fb3",
			"top_pwm_ck"/* parent */, 8),
	GATE_PERI_AO0(CLK_PERAOP_PWM_FB4, "peraop_pwm_fb4",
			"top_pwm_ck"/* parent */, 9),
	GATE_PERI_AO0(CLK_PERAOP_SPI0_B, "peraop_spi0_b",
			"top_spi0_ck"/* parent */, 12),
	GATE_PERI_AO0(CLK_PERAOP_SPI1_B, "peraop_spi1_b",
			"top_spi1_ck"/* parent */, 13),
	GATE_PERI_AO0(CLK_PERAOP_SPI2_B, "peraop_spi2_b",
			"top_spi2_ck"/* parent */, 14),
	GATE_PERI_AO0(CLK_PERAOP_SPI3_B, "peraop_spi3_b",
			"top_spi3_ck"/* parent */, 15),
	GATE_PERI_AO0(CLK_PERAOP_SPI4_B, "peraop_spi4_b",
			"top_spi4_ck"/* parent */, 16),
	GATE_PERI_AO0(CLK_PERAOP_SPI5_B, "peraop_spi5_b",
			"top_spi5_ck"/* parent */, 17),
	GATE_PERI_AO0(CLK_PERAOP_SPI6_B, "peraop_spi6_b",
			"top_spi6_ck"/* parent */, 18),
	GATE_PERI_AO0(CLK_PERAOP_SPI7_B, "peraop_spi7_b",
			"top_spi7_ck"/* parent */, 19),
	GATE_PERI_AO0(CLK_PERAOP_DMA_B, "peraop_dma_b",
			"top_axip_ck"/* parent */, 29),
	/* PERAO1 */
	GATE_PERI_AO1(CLK_PERAOP_SSUSB0_FRMCNT, "peraop_ssusb0_frmcnt",
			"da_univ_48m_div_ck"/* parent */, 1),
	GATE_PERI_AO1(CLK_PERAOP_MSDC0, "peraop_msdc0",
			"top_msdc50_0_ck"/* parent */, 7),
	GATE_PERI_AO1(CLK_PERAOP_MSDC0_H, "peraop_msdc0_h",
			"top_msdc5hclk_ck"/* parent */, 8),
	GATE_PERI_AO1(CLK_PERAOP_MSDC0_FAES, "peraop_msdc0_faes",
			"top_aes_msdcfde_ck"/* parent */, 9),
	GATE_PERI_AO1(CLK_PERAOP_MSDC0_MST_F, "peraop_msdc0_mst_f",
			"top_axip_ck"/* parent */, 10),
	GATE_PERI_AO1(CLK_PERAOP_MSDC0_SLV_H, "peraop_msdc0_slv_h",
			"top_axip_ck"/* parent */, 11),
	GATE_PERI_AO1(CLK_PERAOP_MSDC1, "peraop_msdc1",
			"top_msdc30_1_ck"/* parent */, 12),
	GATE_PERI_AO1(CLK_PERAOP_MSDC1_H, "peraop_msdc1_h",
			"top_msdc30_1_h_ck"/* parent */, 13),
	GATE_PERI_AO1(CLK_PERAOP_MSDC1_MST_F, "peraop_msdc1_mst_f",
			"top_axip_ck"/* parent */, 14),
	GATE_PERI_AO1(CLK_PERAOP_MSDC1_SLV_H, "peraop_msdc1_slv_h",
			"top_axip_ck"/* parent */, 15),
	/* PERAO2 */
	GATE_PERI_AO2(CLK_PERAOP_AUDIO0, "peraop_audio0",
			"top_axip_ck"/* parent */, 0),
	GATE_PERI_AO2(CLK_PERAOP_AUDIO1, "peraop_audio1",
			"top_axip_ck"/* parent */, 1),
	GATE_PERI_AO2(CLK_PERAOP_AUDIO2, "peraop_audio2",
			"top_aud_intbus_ck"/* parent */, 2),
};

static const struct mtk_clk_desc peri_ao_mcd = {
	.clks = peri_ao_clks,
	.num_clks = ARRAY_SIZE(peri_ao_clks),
};

static const struct of_device_id of_match_clk_mt6878_peri_ao[] = {
	{ .compatible = "mediatek,mt6878-pericfg-ao", .data = &peri_ao_mcd },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6878_peri_ao);

static struct platform_driver clk_mt6878_peri_ao_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6878-peri-ao",
		.of_match_table = of_match_clk_mt6878_peri_ao,
	},
};

MODULE_DESCRIPTION("MediaTek MT8196 pericfg_ao clock controller driver");
module_platform_driver(clk_mt6878_peri_ao_drv);
MODULE_LICENSE("GPL");
