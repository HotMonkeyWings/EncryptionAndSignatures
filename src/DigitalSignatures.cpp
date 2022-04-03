#include <NTL/ZZ.h>
#include <string>
#include <iostream>
#include <math.h>

#include "../utils/EncodeUtils.hpp"
#include "../utils/ECCUtils.hpp"
#include "../utils/ElGamalUtils.hpp"
#include "../utils/RSAUtils.hpp"
#include "../utils/sha1.hpp"

using namespace std;
using namespace NTL;

// Generates RSA Siganture
// (d, n) is the private key of the signature owner
// m is the message
ZZ RSASignatureGenerate(ZZ n, ZZ d, string m)
{
    string hash_hex;
    ZZ hash, sign;
    SHA1 checksum;

    // Converts message to hash in hex, then into decimal form
    checksum.update(m);
    hash_hex = checksum.final();
    hash = HexToDecimal(hash_hex);

    PowerMod(sign, hash, d, n);

    cout << "Sign is " << DisplayBase64(sign) << endl;
    return sign;
}

// Verifies if RSA Signature is valid or not.
// (e, n) is the public key of signature owner
// m is the message, sign is the signature
bool RSASignatureVerify(ZZ e, ZZ n, string m, ZZ sign)
{
    string hash_hex;
    ZZ hash, decodedHash;
    SHA1 checksum;

    // Converts message to hash in hex, then into decimal form
    checksum.update(m);
    hash_hex = checksum.final();
    hash = HexToDecimal(hash_hex);

    PowerMod(decodedHash, sign, e, n);

    if (hash == decodedHash)
    {
        cout << "\nSignature is valid." << endl;
        return true;
    }
    cout << "\nWARNING! Signature is invalid." << endl;
    return false;
}

// Demo instance to run RSA Sigature Generation and Verification.
void RSASigantureDemo()
{
    int keySize;
    char option;
    ZZ sign;
    string msg_string;

    cout << "Select RSA Key Size \n(a) 512\n(b) 1024\n\nOption(default=a):";
    cin.ignore(100, '\n');

    option = getchar();
    keySize = option == 'b' ? 1024 : 512;
    cout << "\nYou have chose " << keySize << " bits for the key.\n"
         << endl;

    // (d,n) will be the private key.
    // (e,n) will be the public key.
    auto [d, e, n] = GenerateRSAKeys(keySize);

    // Read the message
    cout << "Enter message to encrypt: ";
    cin.ignore(100, '\n');
    getline(cin, msg_string);

    sign = RSASignatureGenerate(n, d, msg_string);
    RSASignatureVerify(e, n, msg_string, sign);
}

// ELGamal Signature Scheme

// Generate ElGamal Signatures using the Private Key(x)
tuple<ZZ, ZZ> ElGamalSignatureGenerate(ZZ p, ZZ q, ZZ g, ZZ x, string m)
{
    ZZ hash, k, k_inverse, T;
    string hash_hex;
    SHA1 checksum;
    ZZ r, s;

    // Temp variables
    ZZ temp;

    // Hashing the message
    checksum.update(m);
    hash_hex = checksum.final();
    hash = HexToDecimal(hash_hex);

    while (1)
    {
        // Finding k and k_inverse
        RandomBnd(k, q);
        while (GCD(k, q) != 1)
        {
            cout << k << " did not work. Recomputing..." << endl;
            RandomBnd(k, q);
        }

        InvMod(k_inverse, k, q);
        PowerMod(T, g, k, p);

        // Compute r
        r = T % q;
        if (r == 0)
        {
            cout << "r cannot be 0. Recomputing k..." << endl;
            continue;
        }

        // Compute s
        MulMod(s, k_inverse, (hash + x*r), q); // s = k^-1 (h + xr)
        if (s == 0 || GCD(s, q) != 1)
        {
            cout << "s cannot be 0 / inverse should exist. Recomputing k..." << endl;
            continue;
        }

        return {r, s};
    }
}

// Verify ElGamal Signature using the Public Key y
bool ElGamalSignatureVerify(ZZ p, ZZ q, ZZ g, ZZ y, ZZ r, ZZ s, string m) {

    //  Check if parameters are valid
    if (r >= q || s >= q || s < 0 || r < 0){
        cout << "WARNING! Signature is invalid." << endl;
    }

    ZZ hash, decodedHash;
    string hash_hex;
    SHA1 checksum;

    // Temp variables
    ZZ w, u1, u2;
    ZZ g_u1, y_u2;
    ZZ T, r_;

    // Hash the message
    checksum.update(m);
    hash_hex = checksum.final();
    hash = HexToDecimal(hash_hex);

    // Compute w = s^-1 mod q
    InvMod(w, s, q);

    // Compute u1 and u2
    MulMod(u1, hash, w, q);
    MulMod(u2, r, w, q);

    // Compute g^u1 and y^u2
    PowerMod(g_u1, g, u1, p);
    PowerMod(y_u2, y, u2, p);

    // Compute T = g^u1 * y^u2 mod p
    MulMod(T, g_u1, y_u2, p);

    // Compute r_ = T mod q;
    r_ = T % q;

    // Compare r and r_
    if (r == r_) {
        cout << "Signature is Valid." << endl;
        return true;
    }
    cout << "WARNING! Signature is Invalid." << endl;
    return false;
}

// Demo instance to run ElGamal Sigature Generation and Verification.
void ElGamalSignatureDemo()
{
    string msg;
    long keySize;
    char option;

    cout << "Select ElGamal Key Size \n(a) 512\n(b) 1024\n\nOption(default=a):";
    cin.ignore(100, '\n');

    option = getchar();
    keySize = option == 'b' ? 1024 : 512;
    cout << "\nYou have chose " << keySize << " bits for the key.\n"
         << endl;

    // Generate the  DL Parameters
    auto [p, q, g] = GenerateDLParameters(keySize, keySize);
    cout << "p = " << p << "\nq = " << q << "\ng = " << g << endl;

    // Generate the key pair
    auto [x, y] = GenerateDLKeyPair(p, q, g);
    cout << "\nPrivate Key(x): " << DisplayBase64(x) << "\nPublic Key(y): " << DisplayBase64(y) << endl;

    cin.ignore(100, '\n');
    cout << "\nEnter message to sign: ";
    // cin.ignore(100, '\n');
    getline(cin, msg);

    auto [r, s] = ElGamalSignatureGenerate(p, q, g, x, msg);
    cout << "r: " << DisplayBase64(r) << "\ns: " << DisplayBase64(s) << endl;

    ElGamalSignatureVerify(p, q, g, y, r, s, msg);
}

// ECC Signature Scheme

// Generates ECC Signature given a curve E over prime field p
// with order n and Private Key (d)
// Point P is taken as the initial point
tuple<ZZ, ZZ> ECCSignatureGenerate(ZZ p, Curve E, Point P, ZZ n, ZZ d, string m) {
    Point R;
    ZZ k, r, s, hash;
    string hash_hex;
    SHA1 checksum;

    // Hash the message
    checksum.update(m);
    hash_hex = checksum.final();
    hash = HexToDecimal(hash_hex);

    do {
        RandomBnd(k, n);
    } while (k == 0);

    R = ScalarMult(p, E, P, k);
    r = R.x;                    // Take only x-coordinate

    s = (InvMod(k, n) * (hash + d*r)) % n;      // s = k_inverse * (h + dr) mod p

    return {r, s};
}

// Verify the signature is valid using Public Key (Q)
bool ECCSignatureVerify(ZZ p, Curve E, Point P, ZZ n, Point Q, ZZ r, ZZ s, string m) {
    ZZ w, u, v, hash;
    Point R;
    
    // Temp Variables
    string hash_hex;
    SHA1 checksum;
    Point uP, vQ;
    
    // w = s^-1 mod n
    InvMod(w, s, n);


    // Hash the message
    checksum.update(m);
    hash_hex = checksum.final();
    hash = HexToDecimal(hash_hex);

    // Calculate u and v
    MulMod(u, hash, w, n);          // u = h*w mod n
    MulMod(v, r, w, n);             // v = r*w mod n

    // Calculate R
    uP = ScalarMult(p, E, P, u);
    vQ = ScalarMult(p, E, Q, v);
    R = AddPoints(p, E, uP, vQ);
    cout << "\nComputed R.x: " << R.x << endl;

    if (r == R.x) {
        cout << "Signature is Valid." << endl;
        return true;
    }
    cout << "WARNING! Signature is invalid." << endl;
    return true;
}

// Demo instance to run ECC Sigature Generation and Verification.
void ECCSignatureDemo() {
    string msg_string;

    cout << "Enter message to encrypt: ";
    cin.ignore(100, '\n');
    getline(cin, msg_string);
    Point P = {
        ZZ(HexToDecimal("188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012")),
        ZZ(HexToDecimal("07192b95ffc8da78631011ed6b24cdd573f977a11e794811"))
    };
    Curve E = {
        ZZ(-3),
        ZZ(HexToDecimal("64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1"))
    };
    ZZ p = power(ZZ(2), 192) - power(ZZ(2), 64) - ZZ(1);
    ZZ n = ZZ(HexToDecimal("ffffffffffffffffffffffff99def836146bc9b1b4d22831"));

    auto [Q, d] = ECCGenerateKeys(p, E, P, n);
    cout << "\nPrivate Key(d): " << DisplayBase64(d) << endl;
    cout << "Public Key(Q): (" << Q.x << ",\n" << Q.y << ")" << endl;

    auto [r, s] = ECCSignatureGenerate(p, E, P, n, d, msg_string);
    cout << "\nGenerated Signatures:\nr: " << r << "\ns: " << s << endl;

    ECCSignatureVerify(p, E, P, n, Q, r, s, msg_string);
}


int main()
{
    char option;
    cout << "DIGITAL SIGNATURE SCHEMES\n\n(a) RSA Digital Signature\n(b) ElGamal Digital Signature\n(c) ECC Digital Signature\nOption(default=a): ";
    cin >> option;

    switch(option) {
        case 'b':
            ElGamalSignatureDemo();
            break;
        case 'c':
            ECCSignatureDemo();
            break;
        default:
            RSASigantureDemo();
    }
    return 0;
}