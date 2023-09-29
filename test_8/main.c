#include <libdragon.h>

#define TEXT_POS(x, y) ((x)*8), ((y)*8)
#define CX_1 1
#define CX_2 2
#define CX_4 4

// static char s_sb_128[128];

int main(void)
{
    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    controller_init();

    int cursor = 0;

    while (1)
    {
        display_context_t disp = NULL;
        while (!(disp = display_lock()))
            ;

        graphics_fill_screen(disp, 0xECECECFF);
        graphics_set_color(0x1A1D21FF, 0);

        graphics_draw_text(disp, TEXT_POS(CX_4, 1), "Edge coefficients");
        graphics_draw_text(disp, TEXT_POS(CX_4, 2), "lft");
        graphics_draw_text(disp, TEXT_POS(CX_4, 3), "YL");
        graphics_draw_text(disp, TEXT_POS(CX_4, 4), "YM");
        graphics_draw_text(disp, TEXT_POS(CX_4, 5), "YH");

        controller_scan();
        struct controller_data keys = get_keys_down();

        if (keys.c[0].down)
            cursor++;
        if (keys.c[0].up)
            cursor--;
        if (cursor < 0)
            cursor = 4;
        if (cursor > 4)
            cursor = 0;

        graphics_draw_text(disp, TEXT_POS(CX_1, 1 + cursor), "->");

        display_show(disp);
    }
}
