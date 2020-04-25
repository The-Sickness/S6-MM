
/* linux/drivers/video/exynos_decon/panel/dsim_panel.c
 *
 * Header file for Samsung MIPI-DSI LCD Panel driver.
 *
 * Copyright (c) 2013 Samsung Electronics
 * Minwoo Kim <minwoo7945.kim@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/lcd.h>
#include "../dsim.h"

#include "dsim_panel.h"
#include "dsim_backlight.h"
#include "panel_info.h"

#if defined(CONFIG_EXYNOS_DECON_MDNIE_LITE)
#include "mdnie.h"
#if defined(CONFIG_PANEL_S6E3HF2_DYNAMIC)
#include "mdnie_lite_table_zero.h"
#elif defined(CONFIG_PANEL_S6E3HA2_DYNAMIC)
#include "mdnie_lite_table_zerof.h"
#elif defined(CONFIG_PANEL_S6E3HA3_DYNAMIC)
#include "mdnie_lite_table_noble.h"
#elif defined(CONFIG_PANEL_S6E3HF3_DYNAMIC)
#include "mdnie_lite_table_zen.h"
#elif defined(CONFIG_PANEL_S6E3FA3_A8XE)
#include "mdnie_lite_table_a8xe.h"
#endif
#endif

#ifdef CONFIG_FB_DSU
#include <linux/sec_debug.h>
#endif

#if defined(CONFIG_EXYNOS_DECON_MDNIE_LITE)
static int mdnie_lite_write_set(struct dsim_device *dsim, struct lcd_seq_info *seq, u32 num)
{
	int ret = 0, i;

	for (i = 0; i < num; i++) {
		if (seq[i].cmd) {
			ret = dsim_write_hl_data(dsim, seq[i].cmd, seq[i].len);
			if (ret != 0) {
				dsim_err("%s failed.\n", __func__);
				return ret;
			}
		}
		if (seq[i].sleep)
			usleep_range(seq[i].sleep * 1000, seq[i].sleep * 1000);
	}
	return ret;
}

int mdnie_lite_send_seq(struct dsim_device *dsim, struct lcd_seq_info *seq, u32 num)
{
	int ret = 0;
	struct panel_private *panel = &dsim->priv;

	if (panel->state != PANEL_STATE_RESUMED) {
		dsim_info("%s : panel is not active\n", __func__);
		return -EIO;
	}

	mutex_lock(&panel->lock);
	ret = mdnie_lite_write_set(dsim, seq, num);
	mutex_unlock(&panel->lock);

	return ret;
}

int mdnie_lite_read(struct dsim_device *dsim, u8 addr, u8 *buf, u32 size)
{
	int ret = 0;
	struct panel_private *panel = &dsim->priv;

	if (panel->state != PANEL_STATE_RESUMED) {
		dsim_info("%s : panel is not active\n", __func__);
		return -EIO;
	}

	mutex_lock(&panel->lock);
	ret = dsim_read_hl_data(dsim, addr, size, buf);
	mutex_unlock(&panel->lock);

	return ret;
}
#endif

static int dsim_panel_early_probe(struct dsim_device *dsim)
{
	int ret = 0;
	struct panel_private *panel = &dsim->priv;

	panel->ops = dsim_panel_get_priv_ops(dsim);

	if (panel->ops->early_probe) {
		ret = panel->ops->early_probe(dsim);
	}
	return ret;
}

#ifdef CONFIG_FB_DSU
static unsigned int	dsu_param_offset = 0;
static unsigned int	dsu_param_value = 0;
struct dsim_device *dsu_temp_dsim;

static void dsim_dsu_sysfs_handler(struct work_struct *work)
{
	struct dsim_device *dsim = dsu_temp_dsim;
	int ret;
	int i;

	dsim_info("%s called\n", __func__ );

	if( dsim->dsu_param_offset==0 ) {
		pr_err( "%s: failed. offset not exist\n", __func__ );
	}

	for( i=0; i<ARRAY_SIZE(dsu_config); i++ ) {
		if( dsu_config[i].xres == dsim->dsu_xres && dsu_config[i].yres == dsim->dsu_yres ) {
			if( dsim->dsu_param_value != dsu_config[i].value ) {
				dsim->dsu_param_value = dsu_config[i].value;
				ret = sec_set_param((unsigned long) dsim->dsu_param_offset, dsim->dsu_param_value);
				pr_info( "%s:%s,%d,%d\n", __func__, dsu_config[i].id_str, dsim->dsu_param_value, ret );
			}  else {
				pr_info( "%s:%s,%d,%d (same)\n", __func__, dsu_config[i].id_str, dsim->dsu_param_value, ret );
			}
			return;
		}
	}

	pr_err( "%s: search fail. (xres=%d,yres=%d)\n", __func__, dsim->dsu_xres, dsim->dsu_yres );
	return;
}

static int dsim_dsu_sysfs(struct dsim_device *dsim)
{
	int ret = 0;

	dsim_info("%s was called (DSU)\n", __func__);
	dsu_temp_dsim = dsim;
	ret = queue_delayed_work( dsim->dsu_sysfs_wq, &dsim->dsu_sysfs_work, msecs_to_jiffies(50));
	return ret;
}

static int dsim_panel_dsu_cmd(struct dsim_device *dsim)
{
	int ret = 0;
	struct panel_private *panel = &dsim->priv;

	dsim_info("%s was called\n", __func__);
	if (panel->ops->dsu_cmd)
		ret = panel->ops->dsu_cmd(dsim);
	dsim_dsu_sysfs( dsim );
	return ret;
}

static int dsim_panel_init(struct dsim_device *dsim)
{
	int ret = 0;
	struct panel_private *panel = &dsim->priv;

	dsim_info("%s was called (DSU)\n", __func__);
	if (panel->ops->init)
		ret = panel->ops->init(dsim);
	return ret;
}
#endif 

static int dsim_panel_probe(struct dsim_device *dsim)
{
	int ret = 0;
	struct panel_private *panel = &dsim->priv;

	dsim->lcd = lcd_device_register("panel", dsim->dev, &dsim->priv, NULL);
	if (IS_ERR(dsim->lcd)) {
		dsim_err("%s : faield to register lcd device\n", __func__);
		ret = PTR_ERR(dsim->lcd);
		goto probe_err;
	}

	ret = dsim_backlight_probe(dsim);
	if (ret) {
		dsim_err("%s : failed to prbe backlight driver\n", __func__);
		goto probe_err;
	}


	panel->lcdConnected = PANEL_CONNECTED;
	panel->state = PANEL_STATE_RESUMED;
	panel->temperature = NORMAL_TEMPERATURE;
	panel->acl_enable = 0;
	panel->current_acl = 0;
	panel->siop_enable = 0;
	panel->current_hbm = 0;
	panel->current_vint = 0;
	panel->adaptive_control = ACL_STATUS_ON;
	panel->lux = -1;

#ifdef CONFIG_EXYNOS_DECON_LCD_MCD
	panel->mcd_on = 0;
#endif

	dsim->glide_display_size = 0;

	mutex_init(&panel->lock);

	if (panel->ops->probe) {
		ret = panel->ops->probe(dsim);
		if (ret) {
			dsim_err("%s : failed to probe panel\n", __func__);
			goto probe_err;
		}
	}
#if defined(CONFIG_EXYNOS_DECON_LCD_SYSFS)
	lcd_init_sysfs(dsim);
#endif

#if defined(CONFIG_EXYNOS_DECON_MDNIE_LITE)
	if (panel->mdnie_support) {
		mdnie_register(&dsim->lcd->dev, dsim, (mdnie_w)mdnie_lite_send_seq, (mdnie_r)mdnie_lite_read, panel->coordinate, &tune_info);
		panel->mdnie_class = get_mdnie_class();
	}
#endif

#if defined(CONFIG_FB_DSU)
	dsim->dsu_param_value = dsu_param_value;
	dsim->dsu_param_offset = dsu_param_offset;
	dsim->dsu_sysfs_wq = create_singlethread_workqueue( "dsu_sysfs");
	if( dsim->dsu_sysfs_wq ) {
		INIT_DELAYED_WORK( &dsim->dsu_sysfs_work, dsim_dsu_sysfs_handler );
		flush_workqueue( dsim->dsu_sysfs_wq );
	}
	else dsim_err( "%s : dsu_sysfs_wq init fail.\n", __func__ );
#endif

probe_err:
	return ret;
}

static int dsim_panel_displayon(struct dsim_device *dsim)
{
	int ret = 0;
	struct panel_private *panel = &dsim->priv;

#ifdef CONFIG_LCD_ALPM
	mutex_lock(&panel->alpm_lock);
#endif

	if (panel->state == PANEL_STATE_SUSPENED) {
		if (panel->ops->init) {
			ret = panel->ops->init(dsim);
			if (ret) {
				dsim_err("%s : failed to panel init\n", __func__);
				goto displayon_err;
			}
		}
		panel->state = PANEL_STATE_RESUMED;
	}
#ifdef CONFIG_LCD_HMT
	if(panel->hmt_on == HMT_ON)
		hmt_set_mode(dsim, true);
#endif
	dsim_panel_set_brightness(dsim, 1);

	if (panel->ops->displayon) {
		ret = panel->ops->displayon(dsim);
		if (ret) {
			dsim_err("%s : failed to panel display on\n", __func__);
			goto displayon_err;
		}
	}

displayon_err:
#ifdef CONFIG_LCD_ALPM
	mutex_unlock(&panel->alpm_lock);
#endif
	return ret;
}

static int dsim_panel_suspend(struct dsim_device *dsim)
{
	int ret = 0;
	struct panel_private *panel = &dsim->priv;

	if (panel->state == PANEL_STATE_SUSPENED)
		goto suspend_err;

	panel->state = PANEL_STATE_SUSPENDING;

	if (panel->ops->exit) {
		ret = panel->ops->exit(dsim);
		if (ret) {
			dsim_err("%s : failed to panel exit \n", __func__);
			goto suspend_err;
		}
	}
	panel->state = PANEL_STATE_SUSPENED;

suspend_err:
	return ret;
}

static int dsim_panel_resume(struct dsim_device *dsim)
{
	int ret = 0;
	return ret;
}


static int dsim_panel_dump(struct dsim_device *dsim)
{
	int ret = 0;
	struct panel_private *panel = &dsim->priv;

	dsim_info("%s was called\n", __func__);

	if (panel->ops->dump)
		ret = panel->ops->dump(dsim);

	return ret;
}
#ifdef CONFIG_LCD_DOZE_MODE
static int dsim_panel_enteralpm(struct dsim_device *dsim)
{
	int ret = 0;
	struct panel_private *panel = &dsim->priv;

	dsim_info("%s was called\n", __func__);

	if (panel->alpm_support == 0) {
		dsim_info("%s:this panel does not support doze mode\n", __func__);
		return 0;
	}

	if (panel->lcdConnected == PANEL_DISCONNEDTED) {
		dsim_err("%s : %d : panel was not connected\n", __func__, dsim->id);
		return ret;
	}

	if (panel->state == PANEL_STATE_SUSPENED) {
		if (panel->ops->init) {
			ret = panel->ops->init(dsim);
			if (ret) {
				dsim_err("%s : failed to panel init\n", __func__);
				return ret;
			}
		}
		panel->state = PANEL_STATE_RESUMED;
	}

	if (panel->ops->enteralpm) {
		ret = panel->ops->enteralpm(dsim);
		if (ret) {
			dsim_err("ERR:%s:failed to enter alpm \n", __func__);
			return ret;
		}
		panel->curr_alpm_mode = panel->alpm_mode;
	}
	return ret;
}

static int dsim_panel_exitalpm(struct dsim_device *dsim)
{
	int ret = 0;
	struct panel_private *panel = &dsim->priv;

	dsim_info("%s was called\n", __func__);

	if (panel->alpm_support == 0) {
		dsim_info("%s:this panel does not support doze mode\n", __func__);
		return 0;
	}

	if (panel->lcdConnected == PANEL_DISCONNEDTED) {
		dsim_err("%s : %d : panel was not connected\n", __func__, dsim->id);
		return ret;
	}

	if (panel->state == PANEL_STATE_SUSPENED) {
		if (panel->ops->init) {
			ret = panel->ops->init(dsim);
			if (ret) {
				dsim_err("%s : failed to panel init\n", __func__);
				return ret;
			}
		}
		panel->state = PANEL_STATE_RESUMED;
	}

	if (panel->ops->exitalpm) {
		ret = panel->ops->exitalpm(dsim);
		if (ret) {
			dsim_err("ERR:%s:failed to exit alpm \n", __func__);
			return ret;
		}
	}

	panel->curr_alpm_mode = ALPM_OFF;
	return ret;
}
#endif

static struct mipi_dsim_lcd_driver mipi_lcd_driver = {
	.early_probe = dsim_panel_early_probe,
	.probe		= dsim_panel_probe,
	.displayon	= dsim_panel_displayon,
	.suspend	= dsim_panel_suspend,
	.resume		= dsim_panel_resume,
	.dump		= dsim_panel_dump,
#ifdef CONFIG_LCD_DOZE_MODE
	.enteralpm = dsim_panel_enteralpm,
	.exitalpm = dsim_panel_exitalpm,
#endif
#ifdef CONFIG_FB_DSU
	.dsu_cmd = dsim_panel_dsu_cmd,
	.init = dsim_panel_init,
	.dsu_sysfs = dsim_dsu_sysfs,
#endif
};


int dsim_panel_ops_init(struct dsim_device *dsim)
{
	int ret = 0;

	if (dsim)
		dsim->panel_ops = &mipi_lcd_driver;

	return ret;
}

unsigned int lcdtype = 0;
EXPORT_SYMBOL(lcdtype);

static int __init get_lcd_type(char *arg)
{
	get_option(&arg, &lcdtype);

	dsim_info("--- Parse LCD TYPE ---\n");
	dsim_info("LCDTYPE : %x\n", lcdtype);

	return 0;
}
early_param("lcdtype", get_lcd_type);

#ifdef CONFIG_FB_DSU
static int __init get_dsu_config(char *arg)
{
	int ret;

	ret = get_option(&arg, (unsigned int*) &dsu_param_offset);
	if( ret != 2 ) goto err_get_dsu_config;

	ret = get_option(&arg, (unsigned int*) &dsu_param_value);
	if( ret != 1 ) goto err_get_dsu_config;

	pr_info( "%s:%u, %X\n", __func__, dsu_param_value, dsu_param_offset );
	return 0;

err_get_dsu_config:
	pr_err( "%s : option fail(%s,%d)\n", __func__, arg, ret );
	dsu_param_offset = 0;
	dsu_param_value = 0;
	return -EINVAL;
}
early_param("lcdres_offset", get_dsu_config);
#endif

