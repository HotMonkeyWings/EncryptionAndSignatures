#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <NTL/ZZ.h>
#include <tuple>

using namespace std;
using namespace NTL;

#define BASE64_MAP "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

// cout << "In Dec: " << hexadecimalToDecimal(hexString.str()) << "\n";
ZZ HexToDecimal(string hexVal)
{
    int len = hexVal.size();

    // Initializing base value to 1 (16^0 = 1) and result (dec)
    ZZ base, dec;
    dec = 0;
    base = 1;

    // Convert characters to digit from the end (Helps with scaling base)
    for (int i = len - 1; i >= 0; i--)
    {
        // If number, subtract by ascii of '0' = 48
        if (hexVal[i] >= '0' && hexVal[i] <= '9')
        {
            dec += (int(hexVal[i]) - 48) * base;
        }

        // Convert characters [a,f] to [10,15]
        else if (hexVal[i] >= 'a' && hexVal[i] <= 'f')
        {
            dec += (int(hexVal[i]) - 87) * base;
        }

        // Increment base
        base = base * 16;
    }
    return dec;
}

string StringToHex(string rawString)
{
    ostringstream hexString;

    // Encode into 6 bits and convert to hex
    for (int i = 0; i < rawString.size(); ++i)
    {
        hexString << hex << setw(2) << setfill('0') << static_cast<unsigned int>(rawString[i] & 0xff);
    }

    return hexString.str();
}

string HexToString(string hexString)
{
    string rawString;

    // Take hex values in pairs from hexString
    // And convert to ascii characters
    for (int i = 0; i < hexString.size(); i += 2)
    {
        ostringstream hexValue;
        hexValue << hexString[i] << hexString[i + 1];
        int asciiValue;
        conv(asciiValue, HexToDecimal(hexValue.str()));
        rawString.push_back((char)asciiValue);
    }
    return rawString;
}

string DecimalToHex(ZZ hexString)
{
    char reverseHexArray[10000];

    int i = 0;
    while (hexString != 0)
    {

        // Temp value to store remainder
        int temp = hexString % 16;

        // Check whether integer or alphabet required
        if (temp < 10)
        {
            reverseHexArray[i] = temp + 48;
        }
        else
        {
            reverseHexArray[i] = temp + 87;
        }
        i++;
        hexString = hexString / 16;
    }

    // Reverses the array to proper order
    ostringstream decimalString;
    for (int j = i - 1; j >= 0; j--)
        decimalString << reverseHexArray[j];
    return decimalString.str();
}

string Base64Encode(string rawString)
{
    string hexString;

    // Temp values to hold characters.
    // temp2 helps to keep track of padding.
    int temp1 = 0, temp2 = -6;

    // Take characters and group them into sextets
    for (char c : rawString)
    {
        temp1 = (temp1 << 8) + c;
        temp2 += 8;
        while (temp2 >= 0)
        {
            hexString.push_back(BASE64_MAP[(temp1 >> temp2) & 0x3F]);
            temp2 -= 6;
        }
    }

    // Add characters that require padding
    // Convert into sextets
    if (temp2 > -6)
        hexString.push_back(BASE64_MAP[((temp1 << 8) >> (temp2 + 8)) & 0x3F]);

    // Add necessary padding
    while (hexString.size() % 4)
        hexString.push_back('=');
    return hexString;
}

string Base64Decode(string hexString)
{

    string rawString;

    // Obtain base indices of Base64 characters
    vector<int> BASE_INDICES(65, -1);
    for (int i = 0; i < 64; i++)
    {
        BASE_INDICES[BASE64_MAP[i]] = i;
    }

    // Padding character
    BASE_INDICES['='] = -1;

    // temp2 helps to deal with padded characters
    int temp1 = 0, temp2 = -8;

    // Take characters and group them into octets
    for (char c : hexString)
    {
        // Terminate on padding
        if (BASE_INDICES[c] == -1)
        {
            break;
        }
        temp1 = (temp1 << 6) + BASE_INDICES[c];
        temp2 += 6;

        // Deal with padded characters
        if (temp2 >= 0)
        {
            rawString.push_back(char((temp1 >> temp2) & 0xFF));
            temp2 -= 8;
        }
    }
    return rawString;
}

ZZ Encode(string rawString)
{
    string encodedString = Base64Encode(rawString);
    ZZ encryptableValue = HexToDecimal(StringToHex(encodedString));

    return encryptableValue;
}

string Decode(ZZ decimalString)
{
    string decodedString = Base64Decode(HexToString(DecimalToHex(decimalString)));
    return decodedString;
}

string DisplayBase64(ZZ decimalString)
{
    string displayString = Base64Encode(HexToString(DecimalToHex(decimalString)));
    return displayString;
}

// For ECC Curves
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