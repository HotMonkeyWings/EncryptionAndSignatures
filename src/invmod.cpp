#include <NTL/ZZ.h>
#include <iostream>

using namespace NTL;
using namespace std;

int main() {
    ZZ a, b, c, d,  y;
    long x = 1;
    cin >> b;
    cin >> c;
    y = b % c;
    // PowerMod(y, b, x, c);
    a = InvModStatus(d, y, c);
    // InvMod(a, b, c);
    cout << "a= " << a << "\n d= " << d << endl;
}