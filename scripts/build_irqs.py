#!/bin/python3

"""
Python script to generate an ISR (and IRQ) stub in Assembly
"""
template = ""

comments = [
    "0: Divide By Zero Exception",
    "1: Debug Exception",
    "2: Non Maskable Interrupt",
    "3: Breakpoint",
    "4: INTO Exception",
    "5: Out of Bounds",
    "6: Invalid Opcode",
    "7: Coprocessor Not Available",
    "8: Double Fault",
    "9: Coprocessor Segment Overrun",
    "10: Bad TSS",
    "11: Segment Not Present",
    "12: Stack Fault",
    "13: General Protection Fault",
    "14: Page Fault",
    "15: Reserved",
    "16: Floating Point Exception",
    "17: Alignment Check Exception",
    "18: Machine Check Exception",
]

def build_isr_stub(num: int) -> str:
    comment = ""
    if num in range(19,31+2):
        comment = "; {0}: Reserved".format(num)
    else:
        comment = "; {0}".format(comments[num])

    # generate asm stub
    return """
isr{0}:
\t{1}
\tpush byte 0
\tpush byte {0}
\tjmp isr_common
""".format(num, comment)

def build_stubs():
    template = ""
    j = 0
    for i in range(0,31+2):
        j = i-1
        if j > 0:
            template += build_isr_stub(j)

    isr_f = open("isr.asm", "w")
    isr_f.write(template)
    isr_f.close()

build_stubs()