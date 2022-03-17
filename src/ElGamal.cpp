#include <NTL/ZZ.h>
#include <tuple>

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

ZZ decrypt(ZZ c_msg, ZZ p, ZZ key, ZZ q){
    ZZ msg, h;
    PowerMod(h, p, key, q);
    div(msg, c_msg, h);

    return msg;
}

int main()
{
    ZZ m, q, g, key, h, m_test;
    // Bob's variables
    int keySize;
    cout << "Enter Key Size: ";
    cin >> keySize ;
    RandomBits(q, keySize);
    RandomBnd(g, q);
    key = generatePrivateKey(q);
    PowerMod(h, g, key, q);

    cout << "Enter message to encrypt: ";
    cin >> m;
    auto [c, p] = encrypt(m, q, h, g);

    cout << "Encrypted message: " << c << "\n";

    m_test = decrypt(c, p, key, q);

    cout << "Decrypted message: " << m_test << "\n";
    
}