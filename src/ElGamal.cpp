#include <NTL/ZZ.h>
#include <tuple>
#include "HelperUtils.h"

using namespace std;
using namespace NTL;

ZZ generatePrivateKey(ZZ q)
{
    ZZ key;
    RandomBnd(key, q);

    while (GCD(key, q) != 1)
    {
        RandomBnd(key, q);
    }

    return key;
}

// Alice encrypting the message on receiving Bob's public key variables
tuple<ZZ, ZZ> encrypt(ZZ msg, ZZ q, ZZ h, ZZ g)
{
    // p represents g^k and s represents g^ak
    ZZ s, p, c;
    ZZ k = generatePrivateKey(q);
    PowerMod(s, h, k, q);
    PowerMod(p, g, k, q);

    cout << "g^k: " << p << "\n";
    cout << "g^ak: " << s << "\n";

    // Now to encrypt the message
    mul(c, msg, s);
    return {c, p};
}

ZZ decrypt(ZZ c_msg, ZZ p, ZZ key, ZZ q)
{
    ZZ msg, h;
    PowerMod(h, p, key, q);
    div(msg, c_msg, h);

    return msg;
}

int main()
{
    ZZ msg_raw, q, g, key, h, msg_decrypted;
    string msg_string;

    long temp;
    // Bob's variables
    int keySize;
    char option;
    cout << "Select RSA Key Size \n(a) 512\n(b) 1024\n\nOption(default=a):";
    option = getchar();
    keySize = option == 'b' ? 1024 : 512;
    cout << "\nYou have chose " << keySize << " bits for the key.\n" << endl;

    RandomBits(q, keySize);
    RandomBnd(g, q);
    key = generatePrivateKey(q);
    PowerMod(h, g, key, q);

    cout << "Enter message to encrypt: ";
    cin.ignore(100, '\n');
    getline(cin, msg_string);
    msg_raw = Encode(msg_string);

    auto [c, p] = encrypt(msg_raw, q, h, g);

    cout << "\nEncrypted message: " << DisplayBase64(c) << "\n";

    msg_decrypted = decrypt(c, p, key, q);

    msg_string = Decode(msg_decrypted);

    cout << "Decrypted message: " << msg_string << "\n";
}