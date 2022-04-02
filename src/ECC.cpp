#include <NTL/ZZ_p.h>
#include <tuple>
#include "HelperUtils.h"
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace NTL;

typedef struct Point {
    ZZ x, y;
} Point;

Point AddPoints(Point P, Point Q) {
    // Resulting Point
    Point R;

    // Temp Variables
    ZZ slope;

    // Compute slope
    slope = (Q.y - P.y) / (Q.x - P.x);

    // Compute x
    R.x = power(slope, 2) - P.x - Q.x;

    // Compute y
    R.y = slope * (P.x - R.x) - P.y;

    return R;
}

Point PointDoubling(Point P, ZZ a) {
    // Resulting Doubled Point
    Point Double_P;

    // Temp Variables
    ZZ num, den, slope;
    ZZ x_2;

    // Compute slope
    power(x_2, P.x, 2);
    num = 3 * x_2 + a;
    den = 2 * P.y;
    slope = num / den;

    // Compute x
    Double_P.x = power(slope, 2) - (2 * P.x);

    // Compute y
    Double_P.y = (power(slope, 2) * (P.x - Double_P.x) ) - P.y;

    return Double_P;
}

Point ScalarMult(ZZ n, Point P) {
    // Initialize R as (0, 0)
    Point R = {ZZ(0), ZZ(0)};

    // Iter through each bit of n
    for(long i = 0; i < NumBits(n); i++) {

    }
}

// Generate the ECC Public(Q) and Private(d) keys.
// (a,b) = E (The curve)
// (x,y) = P (Point on curve)
tuple<ZZ, ZZ> ECCGenerateKeys(ZZ p, Point E, Point P, ZZ n) {
    ZZ d;

    // Produce Private Key (d) less than n
    RandomBnd(d, n);

    //

}

int main() {
    cout << NumBits(ZZ(10));
}