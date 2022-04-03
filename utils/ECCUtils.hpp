#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

#define INF power(ZZ(2), 4096)

typedef struct Point
{
    ZZ x, y;
} Point;

typedef struct Curve
{
    ZZ a, b;
} Curve;

Point DoublePoint(ZZ p, Curve E, Point P);

// p indicates the field prime
Point AddPoints(ZZ p, Curve E, Point P, Point Q)
{
    // Check if either points are infinity
    if (P.x == ZZ(INF))
    {
        return Q;
    }
    else if (Q.x == ZZ(INF))
    {
        return P;
    }

    // If points are same, Double one of them
    if (P.x == Q.x && P.y == Q.y)
    {
        return DoublePoint(p, E, P);
    }

    // Resulting Point
    Point R;

    // Temp Variables
    ZZ slope;

    // Compute slope
    slope = (Q.y - P.y) * InvMod((Q.x - P.x) % p, p);

    // Compute x
    R.x = (power(slope, 2) - P.x - Q.x) % p;

    // Compute y
    R.y = (slope * (P.x - R.x) - P.y) % p;

    return R;
}

Point DoublePoint(ZZ p, Curve E, Point P)
{
    // Resulting Doubled Point
    Point Double_P;

    // Temp Variables
    ZZ num, den, slope;
    ZZ x_2;

    // Compute slope
    power(x_2, P.x, 2);
    num = 3 * x_2 + E.a;
    den = 2 * P.y;
    slope = num * InvMod(den % p, p);

    // Compute x
    Double_P.x = (power(slope, 2) - (2 * P.x)) % p;

    // Compute y
    Double_P.y = ((slope * (P.x - Double_P.x)) - P.y) % p;

    return Double_P;
}

Point ScalarMult(ZZ p, Curve E, Point P, ZZ n)
{
    // Initialize R as (0, 0)
    Point R = {ZZ(INF), ZZ(INF)};

    // Iter through each bit of n
    for (long i = 0; i < NumBits(n); i++)
    {

        // Extract the bit
        ZZ mask = ZZ(1) << i;
        ZZ masked_n = n & mask;
        ZZ thebit = masked_n >> i;

        if (thebit == 1)
        {
            R = AddPoints(p, E, P, R);
        }
        P = AddPoints(p, E, P, P);
    }

    return R;
}

// Encode the message to a point on the Elliptic Curve.
// m is the message, and it returns a Point Pm
Point ECCEncode(ZZ p, Curve E, ZZ k, ZZ m)
{
    ZZ S;
    Point Pm;

    for (ZZ i = ZZ(0); i < k; i++)
    {
        Pm.x = k * m + i;
        S = (power(Pm.x, 3) + (E.a * Pm.x) + E.b) % p;
        if (Jacobi(S, p) == 1)
        {
            PowerMod(Pm.y, S, (p + 1) / ZZ(4), p);
            return Pm;
        }
    }
    Point inf = {INF, INF};
    return ScalarMult(p, E, inf, m);
}

// Decodes the point on ECC curve back
// To a message
ZZ ECCDecode(Point Pm, ZZ k)
{
    ZZ result;
    div(result, Pm.x, k);
    return result;
}


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