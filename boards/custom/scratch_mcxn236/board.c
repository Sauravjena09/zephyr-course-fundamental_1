#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <stdio.h>

static int board_scratch_mcxn236_init(void) {
    printf("Board Initialized\n");
    return 0;
}

/* Run this function before the kernel fully boots */
SYS_INIT(board_scratch_mcxn236_init, PRE_KERNEL_1, 99);