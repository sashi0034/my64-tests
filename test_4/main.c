// This file is an example of:
// https://github.com/DragonMinded/libdragon/blob/d74706b5962ecbbf4c9faa00075a2843eeebcabe/examples/spritemap/spritemap.c

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>

static uint32_t frame_counter = 0;
static volatile uint32_t timer_counter = 0;

static char s_sb_256[256];

void update_counter(int ovfl)
{
    timer_counter++;
}

int main(void)
{
    int mode = 1;

    /* Initialize peripherals */
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
    rdp_init();
    controller_init();
    timer_init();

    /* Read in single sprite */
    int fp = dfs_open("/mudkip.sprite");
    sprite_t *mudkip = malloc(dfs_size(fp));
    dfs_read(mudkip, 1, dfs_size(fp), fp);
    dfs_close(fp);

    fp = dfs_open("/earthbound.sprite");
    sprite_t *earthbound = malloc(dfs_size(fp));
    dfs_read(earthbound, 1, dfs_size(fp), fp);
    dfs_close(fp);

    fp = dfs_open("/plane.sprite");
    sprite_t *plane = malloc(dfs_size(fp));
    dfs_read(plane, 1, dfs_size(fp), fp);
    dfs_close(fp);

    /* Kick off animation update timer to fire thirty times a second */
    new_timer(TIMER_TICKS(1000000 / 30), TF_CONTINUOUS, update_counter);

    /* Main loop test */
    while (1)
    {
        static display_context_t disp = 0;

        /* Grab a render buffer */
        while (!(disp = display_lock()))
            ;

        /*Fill the screen */
        graphics_fill_screen(disp, 0xFFFFFFFF);

        /* Set the text output color */
        graphics_set_color(0x0, 0xFFFFFFFF);

        switch (mode)
        {
        case 0:
            /* Software spritemap test */
            graphics_draw_text(disp, 20, 20, "Software spritemap test");

            /* Display a stationary sprite of adequate size to fit in TMEM */
            graphics_draw_sprite_trans(disp, 20, 50, plane);

            /* Display a stationary sprite to demonstrate backwards compatibility */
            graphics_draw_sprite_trans(disp, 50, 50, mudkip);

            /* Display walking NESS animation */
            graphics_draw_sprite_stride(disp, 20, 100, earthbound, ((timer_counter / 15) & 1) ? 1 : 0);

            /* Display rotating NESS animation */
            graphics_draw_sprite_stride(disp, 50, 100, earthbound, ((timer_counter / 8) & 0x7) * 2);

            break;
        case 1:
        {
            /* Hardware spritemap test */
            graphics_draw_text(disp, 20, 20, "Hardware spritemap test");

            /* Assure RDP is ready for new commands */
            rdp_sync(SYNC_PIPE);

            /* Remove any clipping windows */
            rdp_set_default_clipping();

            /* Enable sprite display instead of solid color fill */
            rdp_enable_texture_copy();

            /* Attach RDP to display */
            rdp_attach(disp);

            /* Ensure the RDP is ready to receive sprites */
            rdp_sync(SYNC_PIPE);

            /* Load the sprite into texture slot 0, at the beginning of memory, without mirroring */
            rdp_load_texture(0, 0, MIRROR_DISABLED, plane);

            /* Display a stationary sprite of adequate size to fit in TMEM */
            rdp_draw_sprite(0, 20, 50, MIRROR_DISABLED);

            /* Since the RDP is very very limited in texture memory, we will use the spritemap feature to display
               all four pieces of this sprite individually in order to use the RDP at all */
            for (int i = 0; i < 4; i++)
            {
                /* Ensure the RDP is ready to receive sprites */
                rdp_sync(SYNC_PIPE);

                /* Load the sprite into texture slot 0, at the beginning of memory, without mirroring */
                rdp_load_texture_stride(0, 0, MIRROR_DISABLED, mudkip, i);

                /* Display a stationary sprite to demonstrate backwards compatibility */
                rdp_draw_sprite(0, 50 + (20 * (i % 2)), 50 + (20 * (i / 2)), MIRROR_DISABLED);
            }

            /* Ensure the RDP is ready to receive sprites */
            rdp_sync(SYNC_PIPE);

            /* Load the sprite into texture slot 0, at the beginning of memory, without mirroring */
            rdp_load_texture_stride(0, 0, MIRROR_DISABLED, earthbound, ((timer_counter / 15) & 1) ? 1 : 0);

            /* Display walking NESS animation */
            rdp_draw_sprite(0, 20, 100, MIRROR_DISABLED);

            /* Ensure the RDP is ready to receive sprites */
            rdp_sync(SYNC_PIPE);

            /* Load the sprite into texture slot 0, at the beginning of memory, without mirroring */
            rdp_load_texture_stride(0, 0, MIRROR_DISABLED, earthbound, ((timer_counter / 8) & 0x7) * 2);

            /* Display rotating NESS animation */
            rdp_draw_sprite(0, 50, 100, MIRROR_DISABLED);

            /* Inform the RDP we are finished drawing and that any pending operations should be flushed */
            rdp_detach();

            break;
        }
        }

        frame_counter++;
        snprintf(s_sb_256, sizeof(s_sb_256), "Frame: %ld, Timer: %ld\nNESS W: %d, H: %d, HS: %d, vs: %d\n\tleft: %u, right: %lu",
                 frame_counter, timer_counter,
                 earthbound->width, earthbound->height, earthbound->hslices, earthbound->vslices,
                 ((timer_counter / 15) & 1) ? 1 : 0, ((timer_counter / 8) & 0x7) * 2);
        graphics_draw_text(disp, 20, 150, s_sb_256);

        /* Force backbuffer flip */
        display_show(disp);

        /* Do we need to switch video displays? */
        controller_scan();
        struct controller_data keys = get_keys_down();

        if (keys.c[0].A)
        {
            /* Lazy switching */
            mode = 1 - mode;
        }
    }
}
