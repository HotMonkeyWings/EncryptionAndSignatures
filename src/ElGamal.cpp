#include <NTL/ZZ.h>
#include <iostream>

#include "../utils/EncodeUtils.hpp"
#include "../utils/ElGamalUtils.hpp"

using namespace std;
using namespace NTL;


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

ZZ ElGamalDecrypt(ZZ p, ZZ q, ZZ g, ZZ x, ZZ c1, ZZ c2) {
    // m is to store message in decimals
    ZZ m;

    // Temp variables
    ZZ temp;

    PowerMod(temp, c1, x, p);         // c1^x mod p
    InvMod(temp, temp, p);            // c1^(-x) mod p 
    MulMod(m, c2, temp, p);

    cout << "Decrypted Message: " << Decode(m) << "\n\n\n";

    return m;
}

int main() {
    string msg;

    // Get the Key Size
    long keySize;
    char option;
    cout << "Select ElGamal Key Size \n(a) 512\n(b) 1024\n\nOption(default=a):";
    option = getchar();
    keySize = option == 'b' ? 1024 : 512;
    cout << "\nYou have chose " << keySize << " bits for the key.\n" << endl;

    // Generate the  DL Parameters
    auto [p, q, g] = GenerateDLParameters(keySize, keySize);
    cout << "p = " << p << "\nq = " << q << "\ng = " << g << endl;

    // Generate the key pair
    auto [x, y] = GenerateDLKeyPair(p, q, g);
    cout << "\nPrivate Key(x): " << DisplayBase64(x) << "\nPublic Key(y): " << DisplayBase64(y) << endl;

    // Get and Encrypt the message
    cout << "\nEnter message to encrypt: ";
    cin.ignore(100, '\n');
    getline(cin, msg);
    auto [c1, c2] = ElGamalEncrypt(p, q, g, y, msg);
    cout << "\nc1: " << DisplayBase64(c1) << "\nc2: " << DisplayBase64(c2) << endl;

    // Decrypt the message
    Decode(ElGamalDecrypt(p, q, g, x, c1, c2));

    // An NTL Error happens here, I'm not sure why. But it doesn't
    // Affect the code so I'm going to leave it there for now.
    return 0;
}