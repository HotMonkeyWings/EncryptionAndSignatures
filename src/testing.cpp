#include <csignal>
#include <tuple>
#include <iostream>
#include <NTL/ZZ_p.h>

using namespace std;
using namespace NTL;
#define CHARACTERS "HELLO THERE"

int main() {
    // ZZ p = ZZ(4);
    // ZZ_p::init(p);
    // ZZ_p test;
    // test = 123;

    // cout << test << endl;
    // ZZ q = ZZ(2);
    // ZZ_p::init(q);
    // ZZ_p test2;
    // test2 = 0 + test;

    // cout << test2 << endl;

    // ZZ p = ZZ(5);
    // ZZ_p::init(p);
    // ZZ_p hello;
    // ZZ temp1, temp2, temp;
    // temp1 = 5;
    // temp2 = 2;
    // mul(temp, temp1, temp2);
    // add(temp, temp, temp2);
    // hello =  to_ZZ_p(temp);
    // cout << temp << endl;
    // cout << hello << endl;
    ZZ a, b, c;
    a = ZZ(5);
    b = ZZ(3);
    c = ZZ(10);
    cout << divide(a, b) << endl;
    cout << divide(c, a) <<endl;
    
}