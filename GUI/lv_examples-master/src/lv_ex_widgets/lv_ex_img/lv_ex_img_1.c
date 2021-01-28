#include "../../../lv_examples.h"
#if LV_USE_IMG

/*Load the PNG file into buffer. It's still compressed (not decoded)*/

/* Find the image here: https://github.com/lvgl/lv_examples/tree/master/assets */
//LV_IMG_DECLARE(img_cogwheel_argb);
static lv_obj_t *img1;
static lv_obj_t *img2;
static lv_obj_t *img3;
void lv_ex_img_1(void)
{
    /*Create 4 sliders to adjust RGB color and re-color intensity*/
    /* Now create the actual image */
    img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, "P:调试.bin");
    lv_obj_align(img1, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 0, 0);
    img2 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img2, "P:64.bin");
    lv_obj_align(img2, img1, LV_ALIGN_CENTER, 64, 0);
    img3 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img3, "P:128.bin");
    lv_obj_align(img3, img2, LV_ALIGN_CENTER, 128, 0);
    /*Set a non-white background color for the screen to see the alpha is working on the image*/
}

#endif
