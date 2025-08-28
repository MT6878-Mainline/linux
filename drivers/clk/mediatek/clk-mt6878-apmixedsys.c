// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2023 MediaTek Inc.
 * 		 Chuan-wen Chen <chuan-wen.chen@mediatek.com>
 * Copyright (c) 2025 Igor Belwon <igor.belwon@mentallysanemainliners.org>
 */

#include <dt-bindings/clock/mediatek,mt6878-clock.h>
#include <linux/clk.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>

#include "clk-mtk.h"
#include "clk-pll.h"

/* APMIXEDSYS PLL control register offsets */
#define AP_PLL_CON3		0x00C
#define APLL1_TUNER_CON0	0x040
#define APLL2_TUNER_CON0	0x044
#define ARMPLL_LL_CON0		0x204
#define ARMPLL_LL_CON1		0x208
#define ARMPLL_LL_CON2		0x20C
#define ARMPLL_LL_CON3		0x210
#define ARMPLL_BL_CON0		0x214
#define ARMPLL_BL_CON1		0x218
#define ARMPLL_BL_CON2		0x21C
#define ARMPLL_BL_CON3		0x220
#define CCIPLL_CON0		0x224
#define CCIPLL_CON1		0x228
#define CCIPLL_CON2		0x22C
#define CCIPLL_CON3		0x230
#define MAINPLL_CON0		0x304
#define MAINPLL_CON1		0x308
#define MAINPLL_CON2		0x30C
#define MAINPLL_CON3		0x310
#define UNIVPLL_CON0		0x314
#define UNIVPLL_CON1		0x318
#define UNIVPLL_CON2		0x31C
#define UNIVPLL_CON3		0x320
#define MSDCPLL_CON0		0x35C
#define MSDCPLL_CON1		0x360
#define MSDCPLL_CON2		0x364
#define MSDCPLL_CON3		0x368
#define MMPLL_CON0		0x324
#define MMPLL_CON1		0x328
#define MMPLL_CON2		0x32C
#define MMPLL_CON3		0x330
#define UFSPLL_CON0		0x36C
#define UFSPLL_CON1		0x370
#define UFSPLL_CON2		0x374
#define UFSPLL_CON3		0x378
#define APLL1_CON0		0x334
#define APLL1_CON1		0x338
#define APLL1_CON2		0x33C
#define APLL1_CON3		0x340
#define APLL1_CON4		0x344
#define APLL2_CON0		0x348
#define APLL2_CON1		0x34C
#define APLL2_CON2		0x350
#define APLL2_CON3		0x354
#define APLL2_CON4		0x358

#define PLLEN_ALL	0x070
#define PLLEN_ALL_SET	0x074
#define PLLEN_ALL_CLR	0x078

#define FENC_STATUS_CON0	0x03c

#define MT6878_PLL_FMAX		(3800UL * MHZ)
#define MT6878_PLL_FMIN		(1500UL * MHZ)
#define MT6878_INTEGER_BITS	8

#define PLL_SETCLR(_id, _name, _en_set_reg,			\
		   _en_clr_bit, _flags, _pd_reg,		\
		   _pd_shift, _tuner_reg, _tuner_en_reg,	\
		   _tuner_en_bit, _pcw_reg, _pcw_shift,		\
		   _pcwbits) {					\
	.id = _id,						\
	.name = _name,						\
	.reg = 0,						\
	.en_reg = PLLEN_ALL,					\
	.en_set_reg = PLLEN_ALL_SET,				\
	.en_clr_reg = PLLEN_ALL_CLR,				\
	.flags = _flags,					\
	.fmax = MT6878_PLL_FMAX,				\
	.fmin = MT6878_PLL_FMIN,				\
	.pd_reg = _pd_reg,					\
	.pd_shift = _pd_shift,					\
	.tuner_reg = _tuner_reg,				\
	.tuner_en_reg = _tuner_en_reg,			\
	.tuner_en_bit = _tuner_en_bit,				\
	.pcw_reg = _pcw_reg,					\
	.pcw_shift = _pcw_shift,				\
	.pcwbits = _pcwbits,					\
	.pcwibits = MT6878_INTEGER_BITS,			\
}

#define PLL_FENC(_id, _name, _reg, _fenc_sta_ofs, _fenc_sta_bit,\
		_flags, _pd_reg, _pd_shift,		\
		_pcw_reg, _pcw_shift, _pcwbits,		\
		_pll_en_bit) {				\
	.id = _id,					\
	.name = _name,					\
	.reg = _reg,					\
	.fenc_sta_ofs = _fenc_sta_ofs,			\
	.fenc_sta_bit = _fenc_sta_bit,			\
	.flags = _flags,				\
	.fmax = MT6878_PLL_FMAX,			\
	.fmin = MT6878_PLL_FMIN,			\
	.pd_reg = _pd_reg,				\
	.pd_shift = _pd_shift,				\
	.pcw_reg = _pcw_reg,				\
	.pcw_shift = _pcw_shift,			\
	.pcwbits = _pcwbits,				\
	.pcwibits = MT6878_INTEGER_BITS,		\
	.en_reg = PLLEN_ALL,				\
	.en_set_reg = PLLEN_ALL_SET,			\
	.en_clr_reg = PLLEN_ALL_CLR,			\
	.pll_en_bit = _pll_en_bit,			\
	.ops = &mtk_pll_fenc_clr_set_ops,		\
}

struct mtk_pll_desc {
	const struct mtk_pll_data *clks;
	size_t num_clks;
};

static const struct mtk_pll_data apmixed_plls[] = {
	PLL_SETCLR(CLK_APMIXED_ARMPLL_LL, "armpll-ll",
		9, 0, PLL_AO,
		ARMPLL_LL_CON1, 24/*pd*/,
		0, 0, 0/*tuner*/,
		ARMPLL_LL_CON1, 0, 22/*pcw*/),
	PLL_SETCLR(CLK_APMIXED_ARMPLL_BL, "armpll-bl",
		8, 0, PLL_AO,
		ARMPLL_BL_CON1, 24/*pd*/,
		0, 0, 0/*tuner*/,
		ARMPLL_BL_CON1, 0, 22/*pcw*/),
	PLL_SETCLR(CLK_APMIXED_CCIPLL, "ccipll",
		7, 0, PLL_AO,
		CCIPLL_CON1, 24/*pd*/,
		0, 0, 0/*tuner*/,
		CCIPLL_CON1, 0, 22/*pcw*/),
	PLL_FENC(CLK_APMIXED_MAINPLL, "mainpll", MAINPLL_CON0/*base*/, FENC_STATUS_CON0,
		6, PLL_AO, MAINPLL_CON1, 24/*pd*/, MAINPLL_CON1, 0, 22, 0/*tuner*/),
	PLL_FENC(CLK_APMIXED_UNIVPLL, "univpll", UNIVPLL_CON0/*base*/, FENC_STATUS_CON0,
		 5, 0, UNIVPLL_CON1, 24/*pd*/, UNIVPLL_CON1, 0, 22, 0/*tuner*/),
	PLL_SETCLR(CLK_APMIXED_MSDCPLL, "msdcpll",
		4, 0, 0,
		MSDCPLL_CON1, 24/*pd*/,
		0, 0, 0/*tuner*/,
		MSDCPLL_CON1, 0, 22/*pcw*/),
	PLL_FENC(CLK_APMIXED_MMPLL, "mmpll", MMPLL_CON0/*base*/, FENC_STATUS_CON0,
		 3, 0, MMPLL_CON1, 24/*pd*/, MMPLL_CON1, 0, 22, 0/*tuner*/),
	PLL_SETCLR(CLK_APMIXED_UFSPLL, "ufspll",
		2, 0, 0,
		UFSPLL_CON1, 24/*pd*/,
		0, 0, 0/*tuner*/,
		UFSPLL_CON1, 0, 22/*pcw*/),
	PLL_SETCLR(CLK_APMIXED_APLL1, "apll1",
		1, 0, 0,
		APLL1_CON1, 24/*pd*/,
		APLL1_TUNER_CON0, AP_PLL_CON3, 0/*tuner*/,
		APLL1_CON2, 0, 32/*pcw*/),
	PLL_SETCLR(CLK_APMIXED_APLL2, "apll2",
		0, 0, 0,
		APLL2_CON1, 24/*pd*/,
		APLL2_TUNER_CON0, AP_PLL_CON3, 1/*tuner*/,
		APLL2_CON2, 0, 32/*pcw*/),
};

static const struct mtk_pll_desc apmixed_desc = {
	.clks = apmixed_plls,
	.num_clks = ARRAY_SIZE(apmixed_plls),
};

static int clk_mt6878_apmixed_probe(struct platform_device *pdev)
{
	struct clk_hw_onecell_data *clk_data;
	struct device_node *node = pdev->dev.of_node;
	const struct mtk_pll_desc *mcd;
	int r;

	mcd = device_get_match_data(&pdev->dev);
	if (!mcd)
		return -EINVAL;

	clk_data = mtk_alloc_clk_data(mcd->num_clks);
	if (!clk_data)
		return -ENOMEM;

	r = mtk_clk_register_plls(node, mcd->clks, mcd->num_clks, clk_data);
	if (r)
		goto free_apmixed_data;

	r = of_clk_add_hw_provider(node, of_clk_hw_onecell_get, clk_data);
	if (r)
		goto unregister_plls;

	platform_set_drvdata(pdev, clk_data);

	return r;

unregister_plls:
	mtk_clk_unregister_plls(mcd->clks, mcd->num_clks, clk_data);
free_apmixed_data:
	mtk_free_clk_data(clk_data);
	return r;
}

static void clk_mt6878_apmixed_remove(struct platform_device *pdev)
{
	const struct mtk_pll_desc *mcd = device_get_match_data(&pdev->dev);
	struct clk_hw_onecell_data *clk_data = platform_get_drvdata(pdev);
	struct device_node *node = pdev->dev.of_node;

	of_clk_del_provider(node);
	mtk_clk_unregister_plls(mcd->clks, mcd->num_clks, clk_data);
	mtk_free_clk_data(clk_data);
}

static const struct of_device_id of_match_clk_mt6878_apmixed[] = {
	{ .compatible = "mediatek,mt6878-apmixedsys", .data = &apmixed_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6878_apmixed);

static struct platform_driver clk_mt6878_apmixed_drv = {
	.probe = clk_mt6878_apmixed_probe,
	.remove = clk_mt6878_apmixed_remove,
	.driver = {
		.name = "clk-mt6878-apmixed",
		.of_match_table = of_match_clk_mt6878_apmixed,
	},
};
module_platform_driver(clk_mt6878_apmixed_drv);

MODULE_DESCRIPTION("MediaTek MT6878 apmixedsys clocks driver");
MODULE_LICENSE("GPL");
