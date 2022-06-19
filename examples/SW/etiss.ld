/*
// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

// This file was modified by the Chair of Electronic Design Automation, TUM
*/

ENTRY(_start)
SEARCH_DIR(.)


MEMORY
{
  ROM  (rx)  : ORIGIN = 0x10000000, LENGTH = 4M
  RAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 1M
}

/* Stack information variables */
_min_stack     = 0x1000;
_min_heap      = 0x1000;


SECTIONS
{
  .vectors :
  {
      . = ALIGN(4);
      KEEP(*(.vectors))
  } > ROM

  .text : {
      . = ALIGN(4);
      _stext = .;
      *(.text)
      _etext  =  .;
      __CTOR_LIST__ = .;
      LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
      *(.ctors)
      LONG(0)
      __CTOR_END__ = .;
      __DTOR_LIST__ = .;
      LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)
      *(.dtors)
      LONG(0)
      __DTOR_END__ = .;
      *(.lit)
      *(.shdata)
      _endtext = .;
  }  > ROM

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > ROM

  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array ))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > ROM

  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array ))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > ROM

  .rodata : {
      . = ALIGN(4);
      *(.rodata);
      *(.rodata.*)
      *(.srodata)
      *(.srodata.*)
  } > ROM

  .shbss :
  {
      . = ALIGN(4);
      *(.shbss)
  } > RAM

  .data : {
      . = ALIGN(4);
      _data_start = .;
      sdata  =  .;
      _sdata  =  .;
      *(.data);
      *(.data.*)
      *(.sdata2.*)
      edata  =  .;
      _edata  =  .;
  } > RAM

  _bss_start = .;
  .bss :
  {
      . = ALIGN(4);
      *(.bss)
      *(.bss.*)
      *(.sbss)
      *(.sbss.*)
      *(COMMON)
  } > RAM
  _bss_end = .;
  _end = .;
  _heap_start = .;

  __global_pointer$ = MAX(_data_start + 0x800, _bss_end - 0x800);


  /* do not place anything after this address, because the heap starts here! */

  /* ensure there is enough room for stack */
  .stack (NOLOAD): {
      . = ALIGN(4);
      . = . + _min_heap;
      . = . + _min_stack;
      . = ALIGN(4);
  } > RAM

  _stack_end = ORIGIN(RAM) + LENGTH(RAM);
}
