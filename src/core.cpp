
#include "core.hpp"
#include <iostream>

using namespace simp32;
using namespace std;

void Core::tick() {
    unsigned int IR = ram.getWord(registers[PC]);
    registers[PC] += 4;

    if (IR == 0) {
        if (syscallHandler) {
            syscallHandler(*this);
        } else {
            cout << "Got syscall, but no handler has been specified" << endl;
        }
    } else if (IR >> 23 == 1 || IR >> 29 == 1) {
        Opcode opcode = static_cast<Opcode>((IR >> 3) & 0x1F);
        int rd = (IR >> 8) & 0x1F;
        long opperandA = getReg((IR >> 13) & 0x1F);
        long opperandB;
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

unsigned int Core::getReg(unsigned char i) {
    return i == 0 ? 0 : registers[i];
}

void Core::setReg(unsigned char i, unsigned int val) {
    if (i == 0)
        return;
    registers[i] = val;
}

void Core::execType12(Condition cond, Opcode opcode, unsigned char rd, unsigned int opperandA, unsigned int opperandB) {
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

    unsigned int result;

    switch (opcode) {
        case add:
            result = opperandA + opperandB;
            flags[OVERFLOW] = MSB(opperandA) == MSB(opperandB) && MSB(opperandA) != MSB(result);
            flags[CARRY] = result < opperandA;
            flags[ZERO] = result == 0;
            flags[NEGATIVE] = NEG(result);
            setReg(rd, result);
            break;
        case adc:
            result = opperandA + opperandB + (flags[CARRY] ? 1 : 0);
            flags[OVERFLOW] = MSB(opperandA) == MSB(opperandB) && MSB(opperandA) != MSB(result);
            flags[CARRY] = result < opperandA || (result == opperandA && flags[CARRY]);
            flags[ZERO] = result == 0;
            flags[NEGATIVE] = NEG(result);
            setReg(rd, result);
            break;
        case sub:
            result = opperandA + INV(opperandB);
            flags[OVERFLOW] = MSB(opperandA) != MSB(opperandB) && MSB(opperandA) != MSB(result);
            flags[CARRY] = result < opperandA;
            flags[ZERO] = result == 0;
            flags[NEGATIVE] = NEG(result);
            setReg(rd, result);
            break;
        case sbc:
            result = opperandA + INV(opperandB) - (flags[CARRY] ? 1 : 0);
            flags[OVERFLOW] = MSB(opperandA) != MSB(opperandB) && MSB(opperandA) != MSB(result);
            flags[CARRY] = result < opperandA;
            flags[ZERO] = result == 0;
            flags[NEGATIVE] = NEG(result);
            setReg(rd, result);
            break;
        case and_:
            result = opperandA & opperandB;
            flags[OVERFLOW] = 0;
            flags[CARRY] = 0;
            flags[NEGATIVE] = NEG(result);
            flags[ZERO] = result == 0;
            setReg(rd, result);
            break;
        case or_:
            result = opperandA | opperandB;
            flags[OVERFLOW] = 0;
            flags[CARRY] = 0;
            flags[NEGATIVE] = NEG(result);
            flags[ZERO] = result == 0;
            setReg(rd, result);
            break;
        case xor_:
            result = opperandA ^ opperandB;
            flags[OVERFLOW] = 0;
            flags[CARRY] = 0;
            flags[NEGATIVE] = NEG(result);
            flags[ZERO] = result == 0;
            setReg(rd, result);
            break;
        case sll:
            opperandB = opperandB & 0x1F;
            result = (opperandA << opperandB) | (opperandA >> (32 - opperandB));
            flags[OVERFLOW] = 0;
            flags[CARRY] = 0;
            flags[NEGATIVE] = NEG(result);
            flags[ZERO] = result == 0;
            setReg(rd, result);
            break;
        case srl:
            opperandB = opperandB & 0x1F;
            result = (opperandA >> opperandB) | (opperandA << (32 - opperandB));
            flags[OVERFLOW] = 0;
            flags[CARRY] = 0;
            flags[NEGATIVE] = NEG(result);
            flags[ZERO] = result == 0;
            setReg(rd, result);
            break;
        case sra:
            opperandB = opperandB & 0x1F;
            result = (opperandA >> opperandB);
            if (MSB(opperandA))
                result |= 0xFFFFFFFF << (32 - opperandB);
            flags[OVERFLOW] = 0;
            flags[CARRY] = 0;
            flags[NEGATIVE] = NEG(result);
            flags[ZERO] = result == 0;
            setReg(rd, result);
            break;
        case ld:
            result = opperandA + opperandB;
            result = ram.getWord(result);
            setReg(rd, result);
        case ldh:
            result = opperandA + opperandB;
            result = ram.getHalf(result);
            setReg(rd, result);
        case ldb:
            result = opperandA + opperandB;
            result = ram[result];
            setReg(rd, result);
        case st:
            result = opperandA + opperandB;
            ram.setWord(result, getReg(rd));
        case sth:
            result = opperandA + opperandB;
            ram.setHalf(result, getReg(rd));
        case stb:
            result = opperandA + opperandB;
            ram[result] = getReg(rd);
        default:
            cout << "Tried to execute invalid instruction" << endl;
            exit(0);
    }
}