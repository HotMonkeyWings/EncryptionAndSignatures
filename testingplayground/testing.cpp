#include <csignal>
#include <tuple>
#include <iostream>
#include <NTL/ZZ_p.h>

using namespace std;
#define CHARACTERS "HELLO THERE"

int main() {
    ZZ p = ZZ(4);
    ZZ_p::init(p);
    ZZ_p test;
    test = 123;

    cout << test << endl;
}