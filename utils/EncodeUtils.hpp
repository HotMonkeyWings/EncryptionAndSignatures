#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <NTL/ZZ.h>

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
