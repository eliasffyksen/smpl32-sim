
#ifndef CORE_H
#define CORE_H

#include <functional>

#include "ram.hpp"

#define PC 31
#define ZERO 0
#define NEGATIVE 1
#define CARRY 2
#define OVERFLOW 3

#define MSB(x) ((x >> 31) & 1)
#define LSB(x) (x & 1)
#define POS(x) (!MSB(x))
#define NEG(x) MSB(x)
#define INV(x) ((~x) + 1)

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
        and_,
        or_,
        xor_,
        sll,
        srl,
        sra,
        ld = 26,
        ldh,
        ldb,
        st,
        sth,
        stb
    };

    class Core {
        Ram& ram;
        unsigned int registers[32];
        bool flags[4];
        std::function<void(Core&)> syscallHandler = NULL;
        void execType12(Condition cond, Opcode opcode, unsigned char rd, unsigned int opperandA, unsigned int opperandB);
    public:
        Core(Ram& ram) : ram(ram) { registers[PC] = 0; }
        Core(Ram& ram, std::function<void(Core&)> syscallHandler) : ram(ram), syscallHandler(syscallHandler) { registers[PC] = 0; }
        void tick();
        unsigned int getReg(unsigned char i);
        void setReg(unsigned char i, unsigned int val);
    };
}

#endif