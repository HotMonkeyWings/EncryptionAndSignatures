#include <NTL/ZZ.h>
#include <iostream>

// #include "EncodeUtils.hpp"

using namespace std;
using namespace NTL;

// Generate DL Parmaeters
// p = l-bits
// q = q-bits
// g <= q-1
tuple<ZZ, ZZ, ZZ> GenerateDLParameters(long l, long t)
{
    // Initialize DL Parameteres
    ZZ p, q, g;

    // Temp variable
    ZZ h, temp;

    // Computer p such that q divides p-1 using GermainPrimes
    GenGermainPrime(q, t);
    p = 2 * q + 1;

    // Generate g that is not 1
    do
    {
        RandomBnd(h, p);
        div(temp, p - 1, q);
        PowerMod(g, h, temp, p);
    } while (g == 1);

    return {p, q, g};
}

// Generates the DL Key Pair
// x is Private Key
// y is Public Key
tuple<ZZ, ZZ> GenerateDLKeyPair(ZZ p, ZZ q, ZZ g)
{
    ZZ x, y;

    RandomBnd(x, q);
    PowerMod(y, g, x, p);

    return {x, y};
}