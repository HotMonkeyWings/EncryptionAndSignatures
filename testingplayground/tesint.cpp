#include <iostream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

string StringToHex(const string &s, bool upper_case)
{
    ostringstream ret;

    for (string::size_type i = 0; i < s.length(); ++i)
        ret << hex << setfill('0') << setw(2) << (upper_case ? uppercase : nouppercase) << (int)s[i];

    return ret.str();
}

unsigned long HexToLong(const string &s) { return strtoul(s.c_str(), nullptr, 16); }

string HexToString(string hex)
{
    string result = "";
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        string substr = hex.substr(i, 2);

        char ch = HexToLong(substr);

        result += ch;
    }

    return result;
}

string EncodeToLong(const string &s)
{
    ostringstream result;
    string hexString;

    hexString = StringToHex(s, false);
    result << HexToLong(hexString);

    return result.str();
}

string DecodeToText(const string &s)
{
    ostringstream hexString, result;
    hexString << hex << stol(s.c_str()) << endl;
    result << HexToString(hexString.str().c_str());
    return result.str();
}

int main()
{
    string s1;
    string s2;
    ostringstream x, d_h;

    cout << "Enter message: ";
    getline(cin, s1);

    // cout << "string: " << s1 << endl;
    // cout << "hexval: ";

    // s2 = StringToHex(s1, false);

    // cout << s2 << endl;

    // // x.clear();
    // // x.str("");
    // x << HexToLong(s2);
    // cout << x.str() << endl;

    // // Decryption
    // d_h << hex << HexToLong(s2) << endl;

    // cout << d_h.str() << endl;

    // cout << HexToString(d_h.str().c_str()) << endl;

    x << EncodeToLong(s1);
    cout << "Encoded: " << x.str() << endl;
    cout << "Decoded: " << DecodeToText(x.str()) << endl;

    return EXIT_SUCCESS;
}
