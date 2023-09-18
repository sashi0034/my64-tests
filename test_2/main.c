#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>

#include <libdragon.h>
#include <rsp.h>

DEFINE_RSP_UCODE(rsp_process);

static volatile bool s_broke = false;

static void sp_handler()
{
    s_broke = true;
}

static int s_data_buffer[4];

void print_bytes16(unsigned char *origin)
{
    unsigned long i = 0;
    printf("000: ");
    while (i < 16)
    {
        printf("%02X ", origin[i]);
        if (i == 7)
        {
            printf("\n008: ");
        }
        i++;
    }
    printf("\n");
}

int main(void)
{
    /* Initialize peripherals */
    console_init();
    console_set_render_mode(RENDER_MANUAL);
    rsp_init();

    /* Attach SP handler and enable interrupt */
    register_SP_handler(&sp_handler);
    set_SP_interrupt(1);

    rsp_load(&rsp_process);

    unsigned char *origin = malloc(16);
    rsp_read_data(origin, 16, 0);

    print_bytes16(origin);
    console_render();

    // Set data from CPU to RSP
    s_data_buffer[0] = 0x01010101;
    s_data_buffer[1] = 0x10101010;
    s_data_buffer[2] = 0x21212121;

    rsp_load_data(s_data_buffer, sizeof(s_data_buffer), 0); // Write data to DMEM
    rsp_read_data(origin, 16, 0);                           // Read DMEM

    printf("Loaded!\n");
    print_bytes16(origin);
    console_render();

    // Process RSP
    rsp_run_async();
    RSP_WAIT_LOOP(2000)
    {
        if (s_broke)
        {
            break;
        }
    }

    printf("Calculated!\n");

    unsigned char *up = malloc(16);
    rsp_read_data((void *)up, 16, 0);

    print_bytes16(up);
    console_render();
}
