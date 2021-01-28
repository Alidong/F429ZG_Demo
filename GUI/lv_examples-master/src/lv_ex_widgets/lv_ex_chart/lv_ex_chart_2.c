#include "../../../lv_examples.h"
#include "dht11_driver.h"
#include "main_win.h"
#if LV_USE_CHART
extern DHT11_driver DHT11_Data;
lv_obj_t *win;
lv_obj_t *tv;
static lv_obj_t *t1;
static lv_obj_t *t2;
static lv_obj_t *t3;
lv_obj_t *chart1;
lv_obj_t *chart2;
lv_chart_series_t *ser1;
lv_chart_series_t *ser2;
static lv_task_t *task = NULL;
static void chart_cb(lv_task_t *t)
{
    lv_obj_t *chart = t->user_data;
    if (chart == chart1)
    {
        lv_chart_set_next(chart, ser1, (lv_coord_t)DHT11_Data.temperature_float * 100 / 40);
    }
    else
    {
        lv_chart_set_next(chart, ser2, (lv_coord_t)DHT11_Data.humidity_float);
    }
}

//事件回调函数
static void event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == USER_EVENT_BACK) //窗体返回事件,点击左上角的"< Back"按钮会触发此事件
    {
        if (task) //删除掉任务
        {
            lv_task_del(task);
            task = NULL;
        }
    }
}

/**
 * Add a faded area effect to the line chart
 */
void lv_ex_chart_2(void)
{
    /*Create a chart*/

    win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, "CHART");
    //Back_bottn(win);
    lv_obj_set_event_cb(win, chart_cb); //设置事件回调函数
    tv = lv_tabview_create(win, NULL);
    t1 = lv_tabview_add_tab(tv, "Controls");
    t2 = lv_tabview_add_tab(tv, "DHT11");
    t3 = lv_tabview_add_tab(tv, "Selectors");
    lv_coord_t grid_h_chart = lv_page_get_height_grid(t2, 1, 1);
    lv_coord_t grid_w_chart = lv_page_get_width_grid(t2, 1, 1);
    chart1 = lv_chart_create(t2, NULL);
    lv_obj_set_drag_parent(chart1, true);
    lv_obj_set_size(chart1, grid_w_chart, grid_h_chart);
    lv_obj_align(chart1, t2, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(chart1, LV_CHART_TYPE_LINE); /*Show lines and points too*/
    ser1 = lv_chart_add_series(chart1, LV_COLOR_RED);

    lv_obj_set_style_local_pad_left(chart1, LV_CHART_PART_BG, LV_STATE_DEFAULT, 4 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_bottom(chart1, LV_CHART_PART_BG, LV_STATE_DEFAULT, 3 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_right(chart1, LV_CHART_PART_BG, LV_STATE_DEFAULT, 2 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_top(chart1, LV_CHART_PART_BG, LV_STATE_DEFAULT, 2 * (LV_DPI / 10));
    /*Add a faded are effect*/
    lv_obj_set_style_local_value_str(chart1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Temperature");
    lv_obj_set_style_local_bg_opa(chart1, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_OPA_50); /*Max. opa.*/
    lv_obj_set_style_local_bg_grad_dir(chart1, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_obj_set_style_local_bg_main_stop(chart1, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 255); /*Max opa on the top*/
    lv_obj_set_style_local_bg_grad_stop(chart1, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 0);   /*Transparent on the bottom*/
    lv_chart_set_y_tick_length(chart1, 0, 0);
    lv_chart_set_y_tick_texts(chart1, "40\n30\n20\n10\n0", 0, LV_CHART_AXIS_DRAW_LAST_TICK);

    chart2 = lv_chart_create(t2, chart1);
    lv_obj_set_style_local_value_str(chart2, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Humidity");
    ser2 = lv_chart_add_series(chart2, LV_COLOR_BLUE);
    lv_chart_set_y_tick_texts(chart2, "100\n80\n60\n40\n20\n0", 0, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_obj_align(chart2, chart1, LV_ALIGN_OUT_BOTTOM_MID, 0, LV_DPX(30));

    /*Add two data series*/

    // /*Set the next points on 'ser1'*/
    // lv_chart_set_next(chart, ser1, 31);
    // lv_chart_set_next(chart, ser1, 66);
    // lv_chart_set_next(chart, ser1, 10);
    // lv_chart_set_next(chart, ser1, 89);
    // lv_chart_set_next(chart, ser1, 63);
    // lv_chart_set_next(chart, ser1, 56);
    // lv_chart_set_next(chart, ser1, 32);
    // lv_chart_set_next(chart, ser1, 35);
    // lv_chart_set_next(chart, ser1, 57);
    // lv_chart_set_next(chart, ser1, 85);

    // /*Directly set points on 'ser2'*/
    // ser2->points[0] = 92;
    // ser2->points[1] = 71;
    // ser2->points[2] = 61;
    // ser2->points[3] = 15;
    // ser2->points[4] = 21;
    // ser2->points[5] = 35;
    // ser2->points[6] = 35;
    // ser2->points[7] = 58;
    // ser2->points[8] = 31;
    // ser2->points[9] = 53;

    task = lv_task_create(chart_cb, 1000, LV_TASK_PRIO_LOW, chart1);
    task = lv_task_create(chart_cb, 1000, LV_TASK_PRIO_LOW, chart2);
}

#endif
