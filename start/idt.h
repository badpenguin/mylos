; contains an interrupt descriptor
  offset0_15                    equ 0   ; low word of handler offset 15:0
  selector0_15                  equ 2   ; (w) segment selector 15:0
  zero_byte                     equ 4   ; (b) unused in this descriptor format
  flags                         equ 5   ; (b) flag-byte ( P - DPL(2) - 01110 )
  offset16_31                   equ 6   ; (w) high word of handler offset 31:16
