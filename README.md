# CelesteOS
x86_64 Microkernel that aims to be POSIX compliant later in life.

## Building
To build CelesteOS, make sure you have `nasm`, `qemu`, and have built the cross-compiler (using `maketoolchain.sh`). 

Building the OS requires access to a Linux install (I recommend Arch or Ubuntu), and there will be no support added **ever** to build CelesteOS on macOS or Windows based machines.

After you have both a custom cross compiler built and copied to your `PATH` environment variable (or somewhere like `/usr/local/bin`), then the OS can be built and tested in QEMU using `make run`

## Why not just use system GCC?
Using the system GCC compiler is a **horrible** idea when writing an OS since it will try and link in parts of the standard library that we don't have access to, and a whole bunch of other problems. https://wiki.osdev.org/Why_do_I_need_a_Cross_Compiler

## Checklist
[x] Bootable into long mode
[ ] IDT, IRQs and ISRs (half done, need to add IRQs)
[ ] TSS (Task State Segment)
[x] Physical memory manager (PMM)
[ ] Virtual machine manager & paging
