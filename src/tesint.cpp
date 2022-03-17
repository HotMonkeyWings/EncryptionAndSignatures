#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <stdlib.h>
#include <cstdio>
#include <typeinfo>

#include  <bits/stdc++.h>
using namespace std;

string StringToHex(const string& s, bool upper_case /* = true */)
{
    ostringstream ret;

    for (string::size_type i = 0; i < s.length(); ++i)
        ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << (int)s[i];

    return ret.str();
}

unsigned long HexToLong(const string &s) { return strtoul(s.c_str(), nullptr, 16); }

string HexToString(string hex) {
    string result = "";
    for (size_t i = 0; i < hex.length(); i += 2){
        string substr = hex.substr(i, 2);

        char ch = HexToLong(substr);

        result += ch; 
    }

    return result;
}

long StringToLong(const string& s){
    ostringstream result;
    string hexString;

    hexString = StringToHex(s, false);


}


int main(){
    string s1;
    string s2;
    ostringstream x, d_h;

    cout << "Enter message: ";
    getline(cin, s1);

    // cout << "string: " << s1 << endl;
    cout << "hexval: ";

    s2 = StringToHex(s1, false);

    cout << s2 << endl;

    // x.clear();
    // x.str("");
    x << stoul(s2, nullptr, 16);
    cout << x.str() << endl;

    // Decryption
    d_h << hex << stol(x.str().c_str()) << endl;

    cout << d_h.str() << endl;

    cout << HexToString(d_h.str().c_str()) << endl;

    return EXIT_SUCCESS;
}
