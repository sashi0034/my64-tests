#include <libdragon.h>
#include <string.h>

#define TEXT_POS(x, y) ((x)*8), ((y)*8)
#define INCREMENTAL_LINE(line) ((line) + 1)
#define LX_2 2
#define LX_8 8
#define CY_14 14

#define DURATION_6 6
#define DURATION_30 30
#define DURATION_60 60

typedef signed char s8;
typedef short s16;
typedef int s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

static display_context_t s_display = NULL;
static char s_sb_128[64];

static s16 s_v_vs[8] = {0x0000, 0xFFFF, 0x8000, 0x4000, 0xBFFF, 0x2000, 0x6000, 0x9FFF};
static s16 s_v_vt[8] = {0xFFFF, 0xFFFE, 0xFFFD, 0xFFFC, 0xFFFB, 0xFFFA, 0xFFF9, 0xFFF8};
static s16 s_v_vd[8];
static s16 s_v_l[8];

// Note: This is different from actual VABS operation
void simulate_vabs()
{
    for (int i = 0; i < 8; i++)
    {
        *(volatile u32 *)0xA4500008 = (u32)0x18; // For debug (Register AI_CONTROL)

        if (s_v_vs[i] < 0)
        {
            if (s_v_vt[i] == 0x8000)
            {
                s_v_vd[i] = 0x7FFF;
                s_v_l[i] = 0x8000;
            }
            else
            {
                s_v_vd[i] = -s_v_vt[i];
                s_v_l[i] = -s_v_vt[i];
            }
        }
        else if (s_v_vs[i] == 0)
        {
            s_v_vd[i] = 0x0000;
            s_v_l[i] = 0x0000;
        }
        else
        {
            s_v_vd[i] = s_v_vt[i];
            s_v_l[i] = s_v_vt[i];
        }
    }
}

int main(void)
{
    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    controller_init();

    simulate_vabs();

    int count = 0;
    int8_t se8 = -1;
    int16_t se16 = -1;
    int32_t se32 = -1;
    while (1)
    {
        u8 cursor_y = 1;
        while (!(s_display = display_lock()))
            ;

        // {{{(>_<)}}}
        graphics_fill_screen(s_display, 0x1F1F1FFF);
        graphics_set_color(0xFFFF00FF, 0);
        graphics_draw_text(s_display, TEXT_POS(LX_2, cursor_y), "Tests associated with vabs\n");
        cursor_y += INCREMENTAL_LINE(1);

        // {{{(>_<)}}}
        if ((count % DURATION_6) == 0)
        {
            se8 *= -1;
            se16 *= -1;
            se32 *= -1;
        }
        snprintf(s_sb_128, sizeof(s_sb_128), "SE 8-bit, 16-bit, 32-bit:\n\t%02x, %04x, %08x\n",
                 (u8)se8, (u16)se16, (u32)se32);
        graphics_draw_text(s_display, TEXT_POS(LX_2, cursor_y), s_sb_128);
        cursor_y += INCREMENTAL_LINE(2);

        // {{{(>_<)}}}
        if (se16 < 0)
            graphics_draw_text(s_display, TEXT_POS(LX_2, cursor_y), "SE16 < 0");
        else
            graphics_draw_text(s_display, TEXT_POS(LX_2, cursor_y), "SE16 >= 0");
        cursor_y += INCREMENTAL_LINE(1);

        // {{{(>_^)}}}
        u64 v_l_high, v_l_low;
        memcpy(&v_l_high, &s_v_l[0], sizeof(u64));
        memcpy(&v_l_low, &s_v_l[4], sizeof(u64));
        snprintf(s_sb_128, sizeof(s_sb_128), "VABS\n%016llx\n%016llx",
                 v_l_high, v_l_low);
        graphics_draw_text(s_display, TEXT_POS(LX_2, cursor_y), s_sb_128);
        cursor_y += INCREMENTAL_LINE(3);

        // (o'-'o)^-
        display_show(s_display);

        count++;
    }
}
