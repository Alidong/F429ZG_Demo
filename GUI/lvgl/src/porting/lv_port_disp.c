/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "cmsis_os2.h"
#include "dma2d.h"
#include "dma.h"
#include "ILI9488.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
/*********************
 *      DEFINES
 *********************/
bool dma_finish_flag = false;
extern osSemaphoreId_t Screen_DMAHandle;
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void DMA_Transit_Callback(DMA_HandleTypeDef *_hdma);
static void disp_init(void);
//#define Bank1_SRAM3_ADDR 0x68000000
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
#if LV_USE_GPU
static void gpu_blend(lv_disp_drv_t *disp_drv, lv_color_t *dest, const lv_color_t *src, uint32_t length, lv_opa_t opa);
static void gpu_fill(lv_disp_drv_t *disp_drv, lv_color_t *dest_buf, lv_coord_t dest_width,
                     const lv_area_t *fill_area, lv_color_t color);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /* LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed your display drivers `flush_cb` to copy its content to your dispay.
     * The buffer has to be greater than 1 display row
     *
     * There are three buffering configurations:
     * 1. Create ONE buffer with some rows: 
     *      LVGL will draw the display's content here and writes it to your display
     * 
     * 2. Create TWO buffer with some rows: 
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     * 
     * 3. Create TWO screen-sized buffer: 
     *      Similar to 2) but the buffer have to be screen sized. When LVGL is ready it will give the
     *      whole frame to display. This way you only need to change the frame buffer's address instead of
     *      copying the pixels.
     * */

    /* Example for 1) */
    static lv_disp_buf_t draw_buf_dsc_1;
    static lv_color_t draw_buf_1[LV_HOR_RES_MAX * LV_VER_RES_MAX] __attribute__((at(0x68000000)));
    //static lv_color_t draw_buf_2[LV_HOR_RES_MAX * LV_VER_RES_MAX] __attribute__((at(0x6804B000+8)));
    //printf("draw_buf_1=%d\r\n", &draw_buf_1[LV_HOR_RES_MAX * LV_VER_RES_MAX]);
    //static lv_color_t draw_buf_1[LV_HOR_RES_MAX * 10];                        /*A buffer for 10 rows*/
    lv_disp_buf_init(&draw_buf_dsc_1, draw_buf_1, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX); /*Initialize the display buffer*/

    // /* Example for 2) */
    // static lv_disp_buf_t draw_buf_dsc_2;
    // static lv_color_t draw_buf_2_1[LV_HOR_RES_MAX * 10];                                /*A buffer for 10 rows*/
    // static lv_color_t draw_buf_2_1[LV_HOR_RES_MAX * 10];                                /*An other buffer for 10 rows*/
    // lv_disp_buf_init(&draw_buf_dsc_2, draw_buf_2_1, draw_buf_2_1, LV_HOR_RES_MAX * 10); /*Initialize the display buffer*/

    // /* Example for 3) */
    // static lv_disp_buf_t draw_buf_dsc_3;
    // static lv_color_t draw_buf_3_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];                                /*A screen sized buffer*/
    // static lv_color_t draw_buf_3_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];                                /*An other screen sized buffer*/
    // lv_disp_buf_init(&draw_buf_dsc_3, draw_buf_3_1, draw_buf_3_2, LV_HOR_RES_MAX * LV_VER_RES_MAX); /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    lv_disp_drv_t disp_drv;      /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.buffer = &draw_buf_dsc_1;

#if LV_USE_GPU
    /*Optionally add functions to access the GPU. (Only in buffered mode, LV_VDB_SIZE != 0)*/

    /*Blend two color array using opacity*/
    disp_drv.gpu_blend_cb = gpu_blend;

    /*Fill a memory array with a color*/
    disp_drv.gpu_fill_cb = gpu_fill;
#endif

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* Initialize your display and the required peripherals. */
static void disp_init(void)
{
    /*You code here*/
    ILI9488_init();
    HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream0, HAL_DMA_XFER_CPLT_CB_ID, DMA_Transit_Callback);
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_disp_flush_ready()' has to be called when finished. */
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x_start = area->x1;
    int32_t y_start = area->y1;
    int32_t x_end = area->x2;
    int32_t y_end = area->y2;
    uint32_t total_piexl = (x_end - x_start + 1) * (y_end - y_start + 1);
    LCD_SetWindows(x_start, y_start, x_end, y_end);
    if (total_piexl >= 0xFFFF)
    {
        while (total_piexl--)
        {
            LCD_DATA = color_p->full;
            color_p++;
        }
    }
    else
    {
        dma_finish_flag = false;
        HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, (uint32_t)color_p, LCD_DATA_ADDR, total_piexl);
        while (dma_finish_flag == false)
        {
            //osDelay(1);
            delay_us(1);
        }
    }
    /* code */
    // for (uint8_t i = 0; i < times; i++)
    // {
    //     osSemaphoreAcquire(Screen_DMAHandle, osWaitForever);
    //     HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, (uint32_t)(color_p + i * 0xFFFF), LCD_DATA_ADDR, 0xFFFF);
    //     // while (dma_finish_flag == false)
    //     // {
    //     //     osDelay(1);
    //     // }
    //     // dma_finish_flag = false;
    // }

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

/*OPTIONAL: GPU INTERFACE*/
#if LV_USE_GPU

/* If your MCU has hardware accelerator (GPU) then you can use it to blend to memories using opacity
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void gpu_blend(lv_disp_drv_t *disp_drv, lv_color_t *dest, const lv_color_t *src, uint32_t length, lv_opa_t opa)
{
    /*It's an example code which should be done by your GPU*/
    uint32_t i;
    for (i = 0; i < length; i++)
    {
        dest[i] = lv_color_mix(dest[i], src[i], opa);
    }
}

/* If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void gpu_fill(lv_disp_drv_t *disp_drv, lv_color_t *dest_buf, lv_coord_t dest_width,
                     const lv_area_t *fill_area, lv_color_t color)
{
    /*It's an example code which should be done by your GPU*/
    int32_t x, y;
    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
    for (y = fill_area->y1; y <= fill_area->y2; y++)
    {
        for (x = fill_area->x1; x <= fill_area->x2; x++)
        {
            dest_buf[x] = color;
        }
        dest_buf += dest_width; /*Go to the next line*/
    }
}

#endif /*LV_USE_GPU*/
static void DMA_Transit_Callback(DMA_HandleTypeDef *_hdma)
{
    if ((_hdma->Instance) == (hdma_memtomem_dma2_stream0.Instance))
    {
        dma_finish_flag = true;
    }
}
#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
