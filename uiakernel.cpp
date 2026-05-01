#include "uia_kmanager.h"
//define of __attribute__ = start: (thiet lap co ban)
//define of __attribute__ = end; (thiet lap co ban)
// Phan tach nha (Div the house) XD (GDT)-------------------------------
struct gdt_struct
{
	uint16_t lim_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t attr;
	uint8_t base_high;
} attr_packed;
struct gdt_pointer
{
	uint16_t lim;
	uint64_t base;
} attr_packed;

struct gdt_struct gdt[3];
struct gdt_pointer gdt_ptr = {
	.lim = sizeof(gdt) - 1,
	.base = (uint64_t)&gdt
};
void gdt_set_struct(int x, uint8_t access, uint8_t attr)
{
	// cac base && lim nay deu dc set la "section .bss" :)
	// the bases & lims are set to "section .bss" :)
	gdt[x].lim_low = 0;
	gdt[x].base_low = 0;
	gdt[x].base_mid = 0;
	gdt[x].access = access;
	gdt[x].attr = attr;
	gdt[x].base_high = 0;
}
void gdt_init()
{
	//Chung ta se set cac GDT!
	//we're going to set the GDT!
	//Co ban -> Nang cao (Noob -> Pro)
	gdt_set_struct(0, 0, 0);	// Trong rong (NULL)
	gdt_set_struct(1, 0x9A, 0x20);	// Code SEG
	gdt_set_struct(2, 0x92, 0);	// Datos SEG
	
	asm volatile("lgdt %0" : : "m"(gdt_ptr));
}
//Loa bao loi (IDT)-----------------------------------------------------
struct idt_struct
{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t ist;
	uint8_t attr_type;
	uint16_t offset_mid;
	uint32_t offset_high;
	uint32_t zero; // NULL
} attr_packed;
struct idt_pointer
{
	uint16_t lim;
	uint64_t base;
} attr_packed;
struct idt_frame
{
	uint64_t ip;	 // Ko nen nham lan den dia chi IP
	uint64_t cs;	 // Code SEG
	uint64_t flags; // RFLAGS (Status CPU) trang thai cpu
	uint64_t sp; 	// Stack POINTER
	uint64_t ss; 	// Stack SEG
} 
/* 
	[i] CPU day 5 gia tri vao stack khi co ngat say ra!
	[i] (CPU push 5 value in stack if is an interrupt) - Translatec
*/;

struct idt_struct idt[256];
struct idt_pointer idt_ptr = {
	.lim = (uint16_t)(sizeof(idt_struct) * 256) - 1,
	.base = (uint64_t)&idt
};
void idt_set_gate(int i, uint64_t handler, uint16_t selector, uint8_t type_attr)
{
	//offset:
	idt[i].offset_low = (uint16_t)(handler & 0xFFFF);
	idt[i].offset_mid = (uint16_t)((handler >> 16) & 0xFFFF);
	idt[i].offset_high = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
	//more:
	idt[i].selector = selector;
	idt[i].ist = 0;
	idt[i].attr_type = type_attr;
	idt[i].zero = 0;
}
//kernel panic:
void kernel_panic(const char* m)
{
	uia_option::kclear(0x0C);
	uia_option::kprint("(*T_T*) ------- (!) Kernel Panic (!) ------\n", 0x0C);
	uia_option::kprint("Stopped Messenge: ", 0x0C);
	uia_option::kprint(m, 0x0C);
	uia_option::kprint("\nIDT-Pointer: **\nCrashed!\n", 0x0C);
	while(1);
}
//HAM-XY-LY-LOI:
attr_idt void divbyzero_err(struct idt_frame* frame) // 0
{
	uia_option::kprint("(!) Error: Division by zero!\n", 0x0C);
	while(1); // hlt command
}
attr_idt void invalid_opcode_err(struct idt_frame* frame) /*6*/ {
	kernel_panic("(!) Invalid OPCode detected, unknown command!");
}
attr_idt void double_fault_err(struct idt_frame* frame) /*8*/{
	kernel_panic("(!) Double Fault, Kernel Panic!");
}
attr_idt void gpf_err(struct idt_frame* frame) /*13*/ {
	uia_option::kprint("Unknown this ring\n", 0x0C);
	kernel_panic("(!) Unknown ring (GDF executed!)");
}
attr_idt void page_fault_err(struct idt_frame* frame) /*14*/ {
	uia_option::kclear(0x0F);
	kernel_panic("(!) 'there is a SEG intrusion', Page Fault Error!");
}
//HAM-IDT
attr_idt void keyboard_handler(struct idt_frame* frame)
{
	uint8_t sc = uia_option::read_io(0x60);
	if (sc < 0x80) { uia_option::kprint("(!)", 0x0F); }
	__LAPIC_END_IDT;
}
void idt_init()
{
	// Expections -- -- -- -- -- -- -- -- Loi
	idt_set_gate(0, (uint64_t)divbyzero_err, 0x08, 0x8E);
	idt_set_gate(6, (uint64_t)invalid_opcode_err, 0x08, 0x8E);
	idt_set_gate(8, (uint64_t)double_fault_err, 0x08, 0x8E);
	idt_set_gate(13, (uint64_t)gpf_err, 0x08, 0x8E);
	idt_set_gate(14, (uint64_t)page_fault_err, 0x08, 0x8E);
	// IDT actions -- -- -- -- -- -- -- -- Cac hanh dong cua IDT
	idt_set_gate(33, (uint64_t)keyboard_handler, 0x08, 0x8E);
	
	asm volatile("lidt %0" : : "m"(idt_ptr));
	return; 
}

extern "C" void main()
{
	//CALL!
	gdt_init();
	idt_init();
	// xua duoi PIC: [mask 0-2 => PIC CHINH || MASTER PIC]
	uia_option::write_io(0x21, 0xFF);
	uia_option::write_io(0xA1, 0xFF);
	*(volatile uint32_t*)(__LAPIC_SVR) = 0x1FF;
	uia_option::kclear(0x0F);
	asm volatile("sti");

	uia_option::kprint("Hi! I'am from Vietnam && My name is BangGaming8826 (XBOX tag)", 0x0F);
	while(1);
	return;
}
