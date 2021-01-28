/**
 * @file lv_demo_widgets.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_box.h"

/*********************
 *      DEFINES
 *********************/
LV_FONT_DECLARE(my_symbol); //申明字体
#define MY_BACK_SYMBOL "\xEE\x9A\x88"
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void
lv_demo_printer_anim_out_all(lv_obj_t *obj, uint32_t delay);
static void lv_demo_printer_anim_in(lv_obj_t *obj, uint32_t delay);
static void Home_open(void);
static void Settings_open(void);
static void Sensors_open(void);
static void dht11_create(lv_obj_t *parent);
static void slider_event_cb(lv_obj_t *slider, lv_event_t e);

LV_EVENT_CB_DECLARE(icon_settings_event_handler);
LV_EVENT_CB_DECLARE(icon_sensors_event_handler);
LV_EVENT_CB_DECLARE(back_to_home_event_cb);
LV_EVENT_CB_DECLARE(icon_generic_event_cb);
static lv_obj_t *add_back(lv_event_cb_t event_cb);
static lv_style_t style;
void lv_box_demo(void)
{

	/*Create 4 sliders to adjust RGB color and re-color intensity*/
	/* Now create the actual image */
	/*Darken the button when pressed*/
	lv_style_init(&style);
	lv_style_set_image_recolor_opa(&style, LV_STATE_PRESSED, LV_OPA_30);
	lv_style_set_image_recolor(&style, LV_STATE_PRESSED, LV_COLOR_BLACK);
	lv_style_set_text_color(&style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	Home_open();
}
static void Home_open(void)
{
	/*Create an Image button*/
	lv_obj_t *icon = lv_obj_create(lv_scr_act(), NULL);
	lv_obj_set_size(icon, 160, 160);
	lv_obj_set_click(icon, true);
	lv_obj_set_style_local_bg_color(icon, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_obj_set_style_local_bg_color(icon, LV_OBJ_PART_MAIN, LV_STATE_FOCUSED, lv_color_mix(LV_COLOR_WHITE, LV_COLOR_GRAY, 200));
	/*创建 Settings 图标*/
	lv_obj_t *img = lv_img_create(icon, NULL);
	lv_img_set_src(img, "S:.system/image/settings.bin");
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, -10);
	/*Create a label on the Image button*/
	lv_obj_t *label = lv_label_create(icon, NULL);
	lv_label_set_text(label, "Settings");
	lv_obj_align(label, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	lv_obj_set_event_cb(icon, icon_settings_event_handler);
	lv_obj_align(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 1);
	lv_demo_printer_anim_in(icon, 100);
	/*创建 Sensors 图标*/
	icon = lv_obj_create(lv_scr_act(), NULL);
	lv_obj_set_size(icon, 160, 160);
	lv_obj_set_click(icon, true);
	lv_obj_set_style_local_bg_color(icon, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_obj_set_style_local_bg_color(icon, LV_OBJ_PART_MAIN, LV_STATE_FOCUSED, lv_color_mix(LV_COLOR_WHITE, LV_COLOR_GRAY, 200));
	img = lv_img_create(icon, NULL);
	lv_img_set_src(img, "S:.system/image/sensors.bin");
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, -10);
	/*Create a label on the Image button*/
	label = lv_label_create(icon, NULL);
	lv_label_set_text(label, "Sensors");
	lv_obj_align(label, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	lv_obj_align(icon, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 160);
	lv_obj_set_event_cb(icon, icon_sensors_event_handler);
	lv_demo_printer_anim_in(icon, 100);
}
LV_EVENT_CB_DECLARE(icon_settings_event_handler)
{
	if (e == LV_EVENT_PRESSING)
	{ // 控件上单击事件
		LV_LOG_INFO("imgbtn1 Clicked\n");
	}
	else if (e == LV_EVENT_RELEASED)
	{
		LV_LOG_INFO("imgbtn1 Released\n");
		Settings_open();
		//lv_obj_fade_out(ico, 300, 0);
	}

	icon_generic_event_cb(obj, e);
}
LV_EVENT_CB_DECLARE(icon_sensors_event_handler)
{
	if (e == LV_EVENT_PRESSING)
	{ // 控件上单击事件
		LV_LOG_INFO("imgbtn1 Clicked\n");
	}
	else if (e == LV_EVENT_RELEASED)
	{
		LV_LOG_INFO("imgbtn1 Released\n");
		Sensors_open();
		//lv_obj_fade_out(ico, 300, 0);
	}
	icon_generic_event_cb(obj, e);
}

LV_EVENT_CB_DECLARE(icon_generic_event_cb)
{
	if (e == LV_EVENT_PRESSED)
	{
		lv_obj_t *img = lv_obj_get_child_back(obj, NULL);
		lv_obj_t *txt = lv_obj_get_child(obj, NULL);

		lv_anim_t a;
		lv_anim_init(&a);
		lv_anim_set_time(&a, 100);
		lv_anim_set_var(&a, img);
		lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
		lv_anim_set_values(&a, lv_obj_get_y(img), lv_obj_get_y(img) - lv_obj_get_height(img) / 4);
		lv_anim_start(&a);
		lv_anim_set_var(&a, txt);
		lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
		lv_anim_set_values(&a, lv_obj_get_y(txt), lv_obj_get_y(txt) + lv_obj_get_height(txt) / 4);
		lv_anim_start(&a);
	}
	else if (e == LV_EVENT_PRESS_LOST || e == LV_EVENT_RELEASED)
	{
		lv_obj_t *img = lv_obj_get_child_back(obj, NULL);
		lv_obj_t *txt = lv_obj_get_child(obj, NULL);
		lv_anim_t a;
		lv_anim_init(&a);
		lv_anim_set_time(&a, 100);
		lv_anim_set_var(&a, img);

		lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
		lv_anim_set_values(&a, lv_obj_get_y(img), (lv_obj_get_height(obj) - lv_obj_get_height(img)) / 2 - 10);
		lv_anim_start(&a);
		lv_anim_set_var(&a, txt);
		lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
		lv_anim_set_values(&a, lv_obj_get_y(txt), (lv_obj_get_height(img) + lv_obj_get_height(obj)) / 2 - 10);
		lv_anim_start(&a);
	}
}
static lv_obj_t *add_back(lv_event_cb_t event_cb)
{
	// lv_obj_t *btn = lv_btn_create(lv_scr_act(), NULL);
	// lv_obj_set_size(btn, 80, 80);
	// lv_obj_set_pos(btn, 30, 10);
	//lv_obj_t *btn = lv_imgbtn_create(lv_scr_act(), NULL);
	lv_obj_t *btn = lv_btn_create(lv_scr_act(), NULL);
	lv_obj_set_width(btn, lv_obj_get_width_grid(lv_scr_act(), 5, 1));
	lv_obj_set_height(btn, lv_obj_get_height_grid(lv_scr_act(), 8, 1));
	lv_obj_t *lable = lv_label_create(btn, NULL);
	lv_label_set_text(lable, LV_SYMBOL_BACKSPACE);
	lv_obj_align(btn, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 1);
	lv_obj_set_event_cb(btn, event_cb);
	return btn;
}
LV_EVENT_CB_DECLARE(back_to_home_event_cb)
{
	if (e == LV_EVENT_CLICKED)
	{
		lv_demo_printer_anim_out_all(lv_scr_act(), 0);
		Home_open();
	}
}
static void Settings_open(void)
{
	lv_demo_printer_anim_out_all(lv_scr_act(), 100);
	lv_obj_t *back = add_back(back_to_home_event_cb);
	lv_demo_printer_anim_in(back, 100);
}
static void Sensors_open(void)
{

	lv_demo_printer_anim_out_all(lv_scr_act(), 100);
	lv_style_init(&style);
	lv_style_set_value_align(&style, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_LEFT);
	lv_style_set_value_ofs_y(&style, LV_STATE_DEFAULT, -LV_DPX(10));
	lv_style_set_margin_top(&style, LV_STATE_DEFAULT, LV_DPX(30));

	lv_obj_t *tv = lv_tabview_create(lv_scr_act(), NULL);
	lv_obj_t *t1 = lv_tabview_add_tab(tv, "DHT11");

	lv_obj_set_style_local_pad_left(tv, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, LV_HOR_RES / 4);
	lv_obj_t *back = add_back(back_to_home_event_cb);
	lv_demo_printer_anim_in(back, 100);

	// lv_obj_t *t2 = lv_tabview_add_tab(tv, "Visuals");
	// lv_obj_t *t3 = lv_tabview_add_tab(tv, "Selectors");
	dht11_create(t1);
	lv_demo_printer_anim_in(tv, 100);
}
static void lv_demo_printer_anim_out_all(lv_obj_t *obj, uint32_t delay)
{
	lv_obj_t *child = lv_obj_get_child_back(obj, NULL);
	while (child)
	{
		if (child != lv_scr_act())
		{
			lv_anim_t a;
			lv_anim_init(&a);
			lv_anim_set_var(&a, child);
			lv_anim_set_time(&a, 200);
			lv_anim_set_delay(&a, delay);
			lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
			if (lv_obj_get_y(child) < 80)
			{
				lv_anim_set_values(&a, lv_obj_get_y(child),
								   lv_obj_get_y(child) - 200);
			}
			else
			{
				lv_anim_set_values(&a, lv_obj_get_y(child),
								   lv_obj_get_y(child) + 200);

				delay += 200;
			}
			lv_anim_set_ready_cb(&a, lv_obj_del_anim_ready_cb);
			lv_anim_start(&a);
		}
		child = lv_obj_get_child_back(obj, child);
	}
}
static void lv_demo_printer_anim_in(lv_obj_t *obj, uint32_t delay)
{
	if (obj != lv_scr_act())
	{
		lv_anim_t a;
		lv_anim_init(&a);
		lv_anim_set_var(&a, obj);
		lv_anim_set_time(&a, 100);
		lv_anim_set_delay(&a, delay);
		lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
		lv_anim_set_values(&a, lv_obj_get_y(obj) - (LV_VER_RES / 20),
						   lv_obj_get_y(obj));
		lv_anim_start(&a);
		//lv_obj_fade_in(obj, (LV_VER_RES / 20) - 50, delay);
	}
}

static void dht11_create(lv_obj_t *parent)
{
	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);
	lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
	lv_coord_t grid_w = lv_page_get_width_grid(parent, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1);

#if LV_DEMO_WIDGETS_SLIDESHOW == 0
	static const char *btns[] = {"Cancel", "Ok", ""};

	lv_obj_t *m = lv_msgbox_create(lv_scr_act(), NULL);
	lv_msgbox_add_btns(m, btns);
	lv_obj_t *btnm = lv_msgbox_get_btnmatrix(m);
	lv_btnmatrix_set_btn_ctrl(btnm, 1, LV_BTNMATRIX_CTRL_CHECK_STATE);
#endif

	lv_obj_t *h = lv_cont_create(parent, NULL);
	lv_cont_set_layout(h, LV_LAYOUT_PRETTY_MID);
	lv_obj_add_style(h, LV_CONT_PART_MAIN, &style);
	lv_obj_set_drag_parent(h, true);

	lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Basics");

	lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(h, grid_w);
	lv_obj_t *btn = lv_btn_create(h, NULL);
	lv_btn_set_fit2(btn, LV_FIT_NONE, LV_FIT_TIGHT);
	lv_obj_set_width(btn, lv_obj_get_width_grid(h, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));
	lv_obj_t *label = lv_label_create(btn, NULL);
	lv_label_set_text(label, "Button");

	btn = lv_btn_create(h, btn);
	lv_btn_toggle(btn);
	label = lv_label_create(btn, NULL);
	lv_label_set_text(label, "Button");

	lv_switch_create(h, NULL);

	lv_checkbox_create(h, NULL);

	lv_coord_t fit_w = lv_obj_get_width_fit(h);

	lv_obj_t *slider = lv_slider_create(h, NULL);
	lv_slider_set_value(slider, 40, LV_ANIM_OFF);
	lv_obj_set_event_cb(slider, slider_event_cb);
	lv_obj_set_width_margin(slider, fit_w);

	/*Use the knobs style value the display the current value in focused state*/
	lv_obj_set_style_local_margin_top(slider, LV_SLIDER_PART_BG, LV_STATE_DEFAULT, LV_DPX(25));
	lv_obj_set_style_local_value_font(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, lv_theme_get_font_small());
	lv_obj_set_style_local_value_ofs_y(slider, LV_SLIDER_PART_KNOB, LV_STATE_FOCUSED, -LV_DPX(25));
	lv_obj_set_style_local_value_opa(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_value_opa(slider, LV_SLIDER_PART_KNOB, LV_STATE_FOCUSED, LV_OPA_COVER);
	lv_obj_set_style_local_transition_time(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, 300);
	lv_obj_set_style_local_transition_prop_5(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_STYLE_VALUE_OFS_Y);
	lv_obj_set_style_local_transition_prop_6(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_STYLE_VALUE_OPA);

	slider = lv_slider_create(h, slider);
	lv_slider_set_type(slider, LV_SLIDER_TYPE_RANGE);
	lv_slider_set_value(slider, 70, LV_ANIM_OFF);
	lv_slider_set_left_value(slider, 30, LV_ANIM_OFF);
	lv_obj_set_style_local_value_ofs_y(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, -LV_DPX(25));
	lv_obj_set_style_local_value_font(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, lv_theme_get_font_small());
	lv_obj_set_style_local_value_opa(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_event_cb(slider, slider_event_cb);
	lv_event_send(slider, LV_EVENT_VALUE_CHANGED, NULL); /*To refresh the text*/
	if (lv_obj_get_width(slider) > fit_w)
		lv_obj_set_width(slider, fit_w);

	h = lv_cont_create(parent, h);
	lv_cont_set_fit(h, LV_FIT_NONE);
	lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Text input");
#if LV_DEMO_WIDGETS_SLIDESHOW
	tab_content_anim_create(parent);
#endif
}
static void slider_event_cb(lv_obj_t *slider, lv_event_t e)
{
	if (e == LV_EVENT_VALUE_CHANGED)
	{
		if (lv_slider_get_type(slider) == LV_SLIDER_TYPE_NORMAL)
		{
			static char buf[16];
			lv_snprintf(buf, sizeof(buf), "%d", lv_slider_get_value(slider));
			lv_obj_set_style_local_value_str(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, buf);
		}
		else
		{
			static char buf[32];
			lv_snprintf(buf, sizeof(buf), "%d-%d", lv_slider_get_left_value(slider), lv_slider_get_value(slider));
			lv_obj_set_style_local_value_str(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, buf);
		}
	}
}
