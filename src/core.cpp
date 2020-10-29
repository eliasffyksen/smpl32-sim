
#include "core.hpp"
#include <iostream>

using namespace simp32;
using namespace std;

void Core::tick() {
    unsigned int IR = ram.getWord(registers[PC]);
    registers[PC] += 4;

    if (IR == 0) {
        cout << "syscall" << endl;
    } else if (IR >> 23 == 1 || IR >> 29 == 1) {
        Opcode opcode = static_cast<Opcode>((IR >> 3) & 0x1F);
        int rd = (IR >> 8) & 0x1F;
        unsigned int opperandA = getReg((IR >> 13) & 0x1F);
        unsigned int opperandB;
        if (IR >> 23 == 1) {
            opperandB = getReg((IR >> 18) & 0x1F);
        } else {
            opperandB = (IR >> 18) & 0xFFF;
            if (opperandB > 0x7FF) { // signextending
                opperandB |= 0xFFFFF000;
            }
        }
        Condition cond = static_cast<Condition>(IR & 0b111);
        execType12(cond, opcode, rd, opperandA, opperandB);
    }
}

int Core::getReg(unsigned char i) {
    return i == 0 ? 0 : registers[i];
}

void Core::setReg(unsigned char i, int val) {
    if (i == 0)
        return;
    registers[i] = val;
}

void Core::execType12(Condition cond, Opcode opcode, int rd, unsigned int opperandA, unsigned int opperandB) {
    switch (cond) {
        case always:
            break;
        case eq:
            if (!flags[ZERO])
                return;
            break;
        case ne:
            if (flags[ZERO])
                return;
            break;
        case lt:
            if (!flags[NEGATIVE])
                return;
            break;
        case ge:
            if (flags[NEGATIVE])
                return;
            break;
        case c:
            if (!flags[CARRY])
                return;
            break;
        case nc:
            if (flags[CARRY])
                return;
            break;
        case o:
            if (!flags[OVERFLOW])
                return;
            break;
        default:
            break;
    }

    long result;

    switch (opcode) {
        case add:
            result = opperandA + opperandB;
            break;
        case adc:
            result = opperandA + opperandB;
            break;
        case sub:
            result = opperandA - opperandB;
            break;
        case sbc:
            result = opperandA - opperandB;
            break;
    }
}