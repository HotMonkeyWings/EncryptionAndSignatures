#include <NTL/ZZ.h>
#include <tuple>
#include "HelperUtils.h"
#include <string>
#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;
using namespace NTL;

#define K 13


// Generate the ECC Public(Q) and Private(d) keys.
// (a,b) = E (The curve)
// (x,y) = P (Point on curve)
tuple<Point, ZZ> ECCGenerateKeys(ZZ p, Curve E, Point P, ZZ n)
{
    // Produce Private Key (d) less than n
    ZZ d;
    do {
        RandomBnd(d, n);
    } while(d == 0);
    

    // Compute Q = dP as Public Key(Q)
    Point Q = ScalarMult(p, E, P, d);

    return {Q, d};
}

tuple<Point, Point> ECCEncrypt(ZZ p, Curve E, Point P, ZZ n, Point Q, string msg) {
    ZZ msg_raw, k;
    Point M, C1, C2, temp;

    // Convert msg to decimal
    msg_raw = Encode(msg);

    // Encode msg_raw to an ECC point
    M = ECCEncode(p, E, ZZ(K), msg_raw);

    // Select a random value k
    do {
        RandomBnd(k, n);
    } while (k == 0);

    // Compute C1
    C1 = ScalarMult(p, E, P, k);        // C1 = kP

    // Compute C2
    temp = ScalarMult(p, E, Q, k);      // kQ
    C2 = AddPoints(p, E, M, temp);      // C2 = M + kQ

    return {C1, C2};
}

string ECCDecrypt(ZZ p, Curve E, Point P, ZZ d, Point C1, Point C2) {
    Point temp, M;
    ZZ msg_raw;
    string msg;

    temp = ScalarMult(p, E, C1, d);      // temp = dC1
    temp.y = (-temp.y) % p;              // For subtracting

    M = AddPoints(p, E, C2, temp);       // M = C2 - dC1

    // Decode the message with K
    msg_raw = ECCDecode(M, ZZ(K));

    // Derive from decimal form
    msg = Decode(msg_raw);

    return msg;
}

int main()
{
    string msg_string;

    cout << "Enter message to encrypt: ";
    getline(cin, msg_string);
    Point P = {
        ZZ(HexToDecimal("188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012")),
        ZZ(HexToDecimal("07192b95ffc8da78631011ed6b24cdd573f977a11e794811"))
    };
    Curve E = {
        ZZ(-3),
        ZZ(HexToDecimal("64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1"))
    };
    ZZ p = power(ZZ(2), 192) - power(ZZ(2), 64) - ZZ(1);
    ZZ n = ZZ(HexToDecimal("ffffffffffffffffffffffff99def836146bc9b1b4d22831"));

    auto [Q, d] = ECCGenerateKeys(p, E, P, n);
    cout << "\nPrivate Key(d): " << DisplayBase64(d) << endl;
    cout << "Public Key(Q): (" << Q.x << ",\n" << Q.y << ")" << endl;
    auto [C1, C2] = ECCEncrypt(p, E, P, n, Q, msg_string);

    cout << "\nEncrypted Points on ECC:" << endl;
    cout << "C1: (" << C1.x << ",\n" << C1.y << ")" << endl;
    cout << "C2: (" << C2.x << ",\n" << C2.y << ")" << endl;

    cout << "\nDecrypted Message: ";
    cout << ECCDecrypt(p, E, P, d, C1, C2) << endl; 
}