```Hi!

** -- ** -- ** -- ** -- ** -- ** -- ** -- ** --**

A minimal, high-performance 64-bit micro-kernel core written in C++ and Assembly!

(1) . Key Features:
- Architecture: x86_64 (Long Mode execution).
- Memory: 4GB Identity Mapping with Huge Pages.
- Interrupts: Advanced APIC/LAPIC && IDT exception handling.
- Layout: Precisely controlled via custom Linker Scripts.

(2) . Code && Core File:
- uiakernel.cpp : Main Kernel logic;
- gdtasm.s : Boot && System transitions;
- uia_kmanager.h : contains a number of functions for the kernel;
- uia_define.h : contains a definition of the type and a define number to write neatly;
- linker.ld : Linker file.

(3) . License : The kernel use GNU GPL v3.0
(4) . Mission & Goals:
The Mini-UIA kernel is not just "another kernel"; it is designed to be the kernel for:
- Supercomputers: Optimizing raw performance with direct LAPIC and memory control, ... ;
-  Servers: Ensuring stability and high-throughput data management, ... ;
-  Users / Developers: Providing a clean, documented base for future OS enthusiasts to build upon, ... ;
-   Embedded Systems: Lightweight core for specialized hardware and IoT, ...
-   (...)

** -- ** -- ** -- ** -- ** -- ** -- ** -- ** -- ** -- **

(i) The Mini-UIA is a free micro-kernel!
... && Note:
Open for All: Everyone is welcome to contribute code and improve the hạch tâm. Feel free to open Pull Requests!
Free to Use: This kernel is completely free to use, study, and modify under the terms of the GNU GPL v3.0.

^^ readme.template end<README.md, void>();
