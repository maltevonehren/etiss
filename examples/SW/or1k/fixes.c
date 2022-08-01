#include <stdint.h>

#define REG8(add) *((volatile unsigned char *)(add))

extern uint32_t _or1k_board_uart_base;

void _or1k_uart_write(char c)
{
    REG8(_or1k_board_uart_base) = c;
}

void _or1k_uart_interrupt_handler(uint32_t data)
{
    // dummy
}

int _or1k_uart_init(void)
{
    // dummy
    return 0;
}

void or1k_uart_set_read_cb(void (*cb)(char c))
{
    // dummy
}
