/* Copyright (c) 2017  SiFive Inc. All rights reserved.

   This copyrighted material is made available to anyone wishing to use,
   modify, copy, or redistribute it subject to the terms and conditions
   of the FreeBSD License.   This program is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY expressed or implied,
   including the implied warranties of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  A copy of this license is available at
   http://www.opensource.org/licenses.

*/


.text

.global _start
_start:

  # setup "Machine Trap-Vector Base-Address"
  # mode is 0 -> all exceptions go to same code
  la      t0, _trap_vector
  csrw    mtvec, t0

  # Initialize global pointer
  .option push
  .option norelax
  la      gp, __global_pointer$
  .option pop

  # Initialize the stack pointer
  la      sp, __stack

  # Clear the bss segment
  # memset(void* str, int c, size_t n);
  # memset(__bss_start, 0, __bss_start - _end);
  la      a0, __bss_start
  li      a1, 0
  la      a2, _end
  sub     a2, a2, a0
  call    memset

  # Register global termination functions
  # to be called upon exit
  la      a0, __libc_fini_array  
  call    atexit

  # Run global initialization functions
  call    __libc_init_array

  # call user code
  # main(int argc, char *argv[]);
  # main(0, 0);
  # load argc and argv (TODO)
  li      a0, 0
  li      a1, 0
  li      a2, 0 # set envp = NULL just in case
  call    main

  tail    exit


.p2align 2 # align on 4-byte boundary
_trap_vector:
  # maybe save user space registers
  csrr    a0, mcause
  csrr    a1, mepc
  call    _trap_handler_c
  tail    exit # temporary
