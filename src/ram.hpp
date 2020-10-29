
#ifndef RAM_H
#define RAM_H

namespace simp32 {
    class Ram {
        unsigned int size;
        unsigned char *mem;
    public:
        Ram(unsigned int size);
        ~Ram();

        unsigned char& operator[](unsigned int i);

        unsigned int getWord(unsigned int i);
    };
}

#endif