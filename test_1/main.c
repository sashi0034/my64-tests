#include <libdragon.h>

#define TEXT_POS(x, y) ((x)*8), ((y)*8)
#define LX_8 8
#define CY_14 14

static display_context_t s_display = NULL;
static char s_sb_128[64];

int main(void)
{
    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    controller_init();

    int count = 0;
    while (1)
    {
        while (!(s_display = display_lock()))
            ;

        graphics_fill_screen(s_display, 0x1F1F1FFF);
        graphics_set_color(0xFFFF00FF, 0);
        graphics_draw_text(s_display, TEXT_POS(LX_8, CY_14 - 2), "Hello N64!\n");

        snprintf(s_sb_128, sizeof(s_sb_128), "Count: %2d\n", count);
        graphics_draw_text(s_display, TEXT_POS(LX_8, CY_14 + 2), s_sb_128);
        display_show(s_display);

        count++;
    }
}
