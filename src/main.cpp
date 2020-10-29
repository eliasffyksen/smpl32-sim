
#include <iostream>

#include "ram.hpp"
#include "core.hpp"

using namespace std;
using namespace simp32;

int main() {
    Ram ram(1024);
    Core core(ram);
    core.tick();
}