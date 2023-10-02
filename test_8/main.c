#include <libdragon.h>

#define TEXT_POS(x, y) ((x)*8), ((y)*8)
#define CX_1 1
#define CX_2 2
#define CX_4 4

// static char s_sb_128[128];

struct CmdParam
{
    char name[8];
};

const struct CmdParam k_paramEc[] = {
    {"lft"},
    {"YL"},
    {"YM"},
    {"YH"},
    {"XL"},
    {"XLf"},
    {"XLf"}};

const int k_paramEcCount = sizeof(k_paramEc) / sizeof(k_paramEc[0]);

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
        graphics_draw_text(disp, TEXT_POS(CX_2, 1), "Edge coefficients");
        const int paramStartY = 2;
        for (int y = 0; y < k_paramEcCount; ++y)
        {
            graphics_draw_text(disp, TEXT_POS(CX_2, paramStartY + y), k_paramEc[y].name);
        }

        controller_scan();
        struct controller_data keys = get_keys_down();

        if (keys.c[0].down)
            cursor++;
        if (keys.c[0].up)
            cursor--;
        if (cursor < 0)
            cursor = k_paramEcCount - 1;
        if (cursor > k_paramEcCount - 1)
            cursor = 0;

        graphics_draw_text(disp, TEXT_POS(CX_1, paramStartY + cursor), ">");

        display_show(disp);
    }
}
