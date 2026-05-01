#ifndef ___UIA_KMANAGER___
#define ___UIA_KMANAGER___
#pragma once
#include "uia_define.h"

namespace uia_option
{
	//Option this (method = fucntion):
	static volatile uint16_t* _vgabuffer = (volatile uint16_t*)0xB8000;
	static int _spos = 0;

	volatile inline void uia_clear(int);
	volatile void uia_print(const char*, int);
	inline uint8_t read_io(uint16_t);
	inline void write_io(uint16_t, uint8_t);
	
	volatile inline void kclear(int c = 0x0F)
	{
		for (int i = 0; i < 80 * 25; i++) {
			_vgabuffer[i] = (c << 8) | ' ';
		}
	}
	volatile void kprint(const char* m, int c = 0x0F)
	{
		for (int i = 0; m[i] != '\0'; i++) {
			if (m[i] == '\n') {_spos = (_spos / 80 + 1) * 80;}
			else {_vgabuffer[_spos++] = (c << 8) | m[i];}
		}
	}
	inline uint8_t read_io(uint16_t port)
	{
		uint8_t r;
		asm volatile("inb %w1, %b0" : "=a"(r) : "Nd"(port) : "memory");
		return r;
	}
	inline void write_io(uint16_t port, uint8_t val) {
		asm volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
	}
}; //namespace

#endif
