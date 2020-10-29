
#include <iostream>

#include "ram.hpp"

using namespace simp32;
using namespace std;

Ram::Ram(unsigned int size) : size(size) {
    mem = new unsigned char[size];
}

Ram::~Ram() {
    delete mem;
}

unsigned char& Ram::operator[](unsigned int i) {
    if (i >= size) {
        cout << "Memory(" << size << ") out of bounds: " << i << endl;
        exit(1);
    }
    return mem[i];
}

unsigned int Ram::getWord(unsigned int i) {
    unsigned int result = (*this)[i];
    result += (*this)[i + 1] << 8;
    result += (*this)[i + 2] << 16;
    result += (*this)[i + 3] << 24;
    return result;
}