#ifndef ___UIA_DEFINE___
#define ___UIA_DEFINE___
//define of types && . . .= start:
#define uint16_t unsigned short
#define uint8_t unsigned char
#define uint32_t unsigned int
#define uint64_t unsigned long long
#define size_t uint64_t
#define ssize_t signed long long
#define attr_idt __attribute__((interrupt))
#define attr_packed __attribute__((packed))
#define __LAPIC_BASE 0xFEE00000
#define __LAPIC_EOI (__LAPIC_BASE + 0x00B0)
#define __LAPIC_SVR (__LAPIC_BASE + 0x00F0)
#define __LAPIC_END_IDT *(volatile uint32_t*)(__LAPIC_EOI) = 0
#endif
