
#ifndef CORE_H
#define CORE_H

#include "ram.hpp"

#define PC 31
#define ZERO 0
#define NEGATIVE 1
#define CARRY 2
#define OVERFLOW 3

namespace simp32 {

    enum Condition {
        always,
        eq,
        ne,
        lt,
        ge,
        c,
        nc,
        o
    };

    enum Opcode {
        add,
        adc,
        sub,
        sbc,
        and,
        or,
        xor,
        sll,
        srl,
        sra,
        ldw = 26,
        ldh,
        ldb,
        stw,
        sth,
        stb
    }

    class Core {
        Ram& ram;
        long registers[32];
        bool flags[4];
        void execType12(Condition cond, Opcode opcode, int rd, unsigned int opperandA, unsigned int opperandB);
    public:
        Core(Ram& ram) : ram(ram) { registers[PC] = 0; }
        void tick();
        int getReg(unsigned char i);
        void setReg(unsigned char i, int val);
    };
}

#endif