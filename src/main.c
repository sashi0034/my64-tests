#include <libdragon.h>

static display_context_t s_display = NULL;

int main(void)
{
    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    controller_init();

    while (1)
    {
        while (!(s_display = display_lock()))
            ;

        graphics_fill_screen(s_display, 0x1f1f1fff);
        graphics_set_color(0xFFFF00FF, 0);
        graphics_draw_text(s_display, 20, 20, "Hello N64!\n");
        display_show(s_display);
    }
}
