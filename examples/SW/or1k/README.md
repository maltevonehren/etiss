# ETISS OR1K

Since libglosses OR1K crt0.S script is already very detailed and targeted at running without an execution environment. 
For this reason there is very little setup needed (compared to riscv / no linker script etc.).

We use the `or1ksim-uart.S` board for memory size and "uart" location.

The only custom code is used to modify the uart printing mechanism to work with the ETISS logger plugin.

## printing

Printing is provided by overriding the _or1k_uart_write function with a function that writes to the logger address.
Reading is not implemented.

## exit

exit is currently just a loop to self.
