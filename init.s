.globl _start
_start:
  mov sp,#0x8000
  mov r0,pc
  bl blink
hang: b hang
