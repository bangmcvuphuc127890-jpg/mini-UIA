; 1 + 1 = 2; sigma(1, 100) = 5050; nay . . .
; ***************************[—————]*******************************

[bits 32]
section .multiboot
align 4
multiboot_header:
    dd 0x1BADB002             ; Magic
    dd 0x00                   ; Flags[cite: 1]
    dd - (0x1BADB002 + 0x00)  ; Checksum[cite: 1]

section .text
global _start
_start:
    cli                 
    mov esp, stack_top        ; Thiết lập ngăn xếp

    ; --- THIẾT LẬP BẢNG TRANG CƠ BẢN ĐỂ VÀO LONG MODE ---
    ;4 -> 3
    mov edi, p4_table
    mov eax, p3_table
    or eax, 0b11              ; Present + Writable
    mov [p4_table], eax
    ;3 -> 2
    mov eax, p2_table
    or eax, 0b11              ; Present + Writable
    mov [p3_table], eax

    mov eax, 0x40000000 | 0b10000011
    mov [p3_table + 8], eax
    mov eax, 0x80000000 | 0b10000011
    mov [p3_table + 16], eax
    mov eax, 0xC0000000 | 0b10000011
    mov[p3_table + 24], eax

    mov eax, 0 | 0b10000011   ; Identity Map 2MB đầu tiên (Huge Page)
    mov [p2_table], eax

    mov eax, 0xFE000000 | 0b10000011
    mov [p2_table + 503 * 8], eax
    ; Kích hoạt các luồng hệ thống
    mov eax, p4_table
    mov cr3, eax              ; Nạp địa chỉ bảng trang

    mov eax, cr4
    or eax, 1 << 5            ; Bật PAE
    mov cr4, eax

    mov ecx, 0xC0000080       ; EFER MSR
    rdmsr
    or eax, 1 << 8            ; LME bit
    wrmsr

    mov eax, cr0
    or eax, 1 << 31           ; Bật Paging
    mov cr0, eax

    ; Nhảy thẳng vào hàm trung gian để chuẩn bị vào x64!
    extern main
    call main                 ; Nhảy vào uiakernel.cpp
    hlt

section .bss
align 4096
p4_table: resb 4096
p3_table: resb 4096
p2_table: resb 4096

align 16
stack_bottom:
    resb 4096 * 2             ; ...
stack_top:
