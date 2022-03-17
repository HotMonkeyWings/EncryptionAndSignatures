#include <iostream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

string StringToHex(const string &s, bool upper_case)
{
    ostringstream result;

    for (string::size_type i = 0; i < s.length(); ++i)
        result << hex << setfill('0') << setw(2) << (upper_case ? uppercase : nouppercase) << (int)s[i];

    return result.str();
}

unsigned long HexToLong(const string &s) { return strtoul(s.c_str(), nullptr, 16); }

string HexToString(string hex)
{
    string result = "";
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        string substr = hex.substr(i, 2);

        char letter = HexToLong(substr);

        result += letter;
    }

    return result;
}

unsigned long EncodeToLong(const string &s)
{
    string hexString;

    hexString = StringToHex(s, false);
    return HexToLong(hexString);
}

string DecodeToText(long ul)
{
    ostringstream hexString, result;
    hexString << hex << ul << endl;
    result << HexToString(hexString.str().c_str());
    return result.str();
}