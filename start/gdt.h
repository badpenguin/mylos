
; GDT Constant Offset (1)
        gdt_limit1              equ 0
        gdt_base1               equ 2
        gdt_base2               equ 4
        gdt_type1               equ 5
        gdt_type2               equ 6
        gdt_base3               equ 7

; GDT Constant Offset (2)
        Seg_limit               equ 0           ; Segment Limit 15:00
        Base_A15_A00            equ 2           ; Segment Base Address 15:00
        Base_A23_A16            equ 4           ; Segment Base Address 23:16
        Access_rights           equ 5           ; P - DPL - DT - Type - A
        ; P - Present bit (0=not present, 1=present)
        ; DPL(2) - Descriptor Privilege Level (0=Kernel, 3=Application)
        ; DT - Descriptor Type (0=System-Segment or Gate, 1=Data or Code)
        ; A - Accessed bit
        ; Type(3) - Segment type
        ; Bit   2   1   0   Type        Description
        ; Name  T   E   W   
        ;       0   0   0   Data read-only
        ;       0   0   1   Data read/write
        ;       0   1   0   Data read-only, expand down
        ;       0   1   1   Data read-write, expansion down
        ;       1   0   0   Code exec-only
        ;       1   0   1   Code exec-read
        ;       1   1   0   Code exec-only, conforming
        ;       1   1   1   Code exec-read, conforming
        ; Data Segment (T=0):
        ;       E - Expand-down bit
        ;               For expand-down data segments, the limit has the same 
        ;               function but is interpreted differently. In these 
        ;               cases the range of valid addresses is from limit+1 to 
        ;               either 64K or 2^(32) - 1 (4 Gbytes) depending on the 
        ;               B-bit. An Expand-down segment has maximum size when 
        ;               the limit is zero.
        ;       W - Writable bit
        ; Code Segment (T=1)
        ;       E - Conforming bit (C)
        ;               Conforming means that a segment with C=1 can call 
        ;               another segment with a lower or equal Privilege Level. 
        ;               The Current Privilege Level however isn't changed !
        ;       R - Readable
        GDLimit_A19_A16         equ 6           ; G - B/D - 0 - AVL - Seg. Limit 19:16
        ; G - Granularity bit (0=1 byte, 1=4 KB)
        ; B - Big bit in data segment 
        ; or D Default Operand Size (0=16bit, 1=32bit) in code segment
        ; AVL - Available for System.
        Base_A31_A24            equ 7           ; Segment Base Address 31:24


; SELECTORS USED
; Format of the selector:
;       15-3 Pointer into the GDT
;          2 TI  - (0=GDT, 1=LDT)
;        1-0 RPL - Request Privilege Level (0=Kernel)

        selector_null           equ 0   ; gdt(0) - 0
        selector_kernel_CS      equ 8   ; gdt(1) - 8
        selector_kernel_DS      equ 16  ; gdt(2) - 16
        selector_RAW            equ 24  ; gdt(3) - 24
        selector_code16         equ 32  ; gdt(4) - 32
        selector_data16         equ 40  ; gdt(5) - 40
        selector_kernel_XS      equ 48  ; gdt(6) - 48
        selector_task0          equ 56
	selector_stack0		equ 64
        selector_stack1         equ 73  ; 72 + 1
        selector_stack2         equ 82  ; 80 + 2
        selector_terminal       equ 91  ; 88 + 3
	selector_irq32		equ 96
	selector_kernel32_CS	equ 104
	selector_kernel32_DS	equ 112


stack0_limit 	equ 1024
stack1_limit	equ 1024
stack2_limit	equ 1024
