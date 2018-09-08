CRT_LineFeed:
        mov ax, selector_kernel_DS
        mov FS, ax
        mov word [FS:KCRT_CursorX], 0
        add word [FS:KCRT_CursorY], 1
        retf


