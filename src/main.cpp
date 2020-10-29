
#include <iostream>

#include "ram.hpp"
#include "core.hpp"

using namespace std;
using namespace simp32;

void syscall(Core &core) {
    cout << "SYSCALL HANDLER!!!" << endl;
}

int main() {
    Ram ram(1024);
    Core core(ram, syscall);
    core.tick();
}