#include <NTL/ZZ_p.h>
#include <tuple>
#include "HelperUtils.h"
#include <iostream>

using namespace std;
using namespace NTL;

// Generate the DL parameters (p, q, g)
// q = t-bits, p = l-bits
tuple<ZZ, ZZ, ZZ> GenerateDLParameters(long l, long t)
{
    // Initialize DL Parameteres
    ZZ p, q, g;

    // Temp variable
    ZZ h, temp;

    // Find p such that q divides p-1
    RandomBits(q, t);
    RandomBits(p, l);
    while (divide(p - 1, q) != 1)
    {
        cout << "p = " << p << " did not work. Recomputing..." << endl;
        rem(temp, p - 1, q);
        p = p - temp;

        // Verify if p is still l-bit
        if (NumBits(p) != l) {
            RandomBits(p, l);
        }
    }

    // Generate g that is not 1
    do
    {
        RandomBnd(h, p);
        div(temp, p - 1, q);
        PowerMod(g, h, temp, p);
    } while (g == 1);

    return {p, q, g};
}

tuple<ZZ, ZZ> GenerateDlKeyPair(ZZ p, ZZ q, ZZ g) {
    // x is Private Key
    // y is Public Key
    ZZ x, y;

    RandomBnd(x, q);
    PowerMod(y, g, x, p);

    return {x, y};
}

tuple<ZZ, ZZ> ElGamalEncrypt(ZZ p, ZZ q, ZZ g, ZZ y, string msg) {
    ZZ k, m, c1, c2;
    RandomBnd(k, q);

    // Encode the message
    m = Encode(msg);

    // Encryption Phase
    PowerMod(c1, g, k, p);
    PowerMod(c2, y, k, p);
    MulMod(c2, m, c2, p);

    return {c1, c2};
}

string ElGamalDecrypt(ZZ p, ZZ q, ZZ g, ZZ x, ZZ c1, ZZ c2) {
    string msg;

    // Temp variables
    // m is to store message in decimals
    ZZ temp, m;

    PowerMod(temp, c1, x, p);       // c1^x mod p
    InvMod(temp, temp, p);            // c1^(-x) mod p 
    MulMod(m, c2, temp, p);

    msg = Decode(m);
    return msg;
}

int main() {
    string msg;

    long keySize;
    char option;
    cout << "Select RSA Key Size \n(a) 512\n(b) 1024\n\nOption(default=a):";
    option = getchar();
    keySize = option == 'b' ? 1024 : 512;
    cout << "\nYou have chose " << keySize << " bits for the key.\n" << endl;

    auto [p, q, g] = GenerateDLParameters(keySize, keySize);

    cout << "p = " << p << "\nq = " << q << "\ng = " << g << endl;

    auto [x, y] = GenerateDlKeyPair(p, q, g);

    cout << "\nPrivate Key(x): " << DisplayBase64(x) << "\nPublic Key(y): " << DisplayBase64(y) << endl;

    cout << "\nEnter message to encrypt: ";
    cin.ignore(100, '\n');
    getline(cin, msg);

    auto [c1, c2] = ElGamalEncrypt(p, q, g, y, msg);

    cout << "\nc1: " << DisplayBase64(c1) << "\nc2: " << DisplayBase64(c2) << endl;

    cout << "\nDecrypted Message: " << ElGamalDecrypt(p, q, g, x, c1, c2) << endl;

    return 0;
}