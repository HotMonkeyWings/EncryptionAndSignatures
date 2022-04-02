
#include <NTL/ZZ_p.h>
#include <csignal>
#include <tuple>
#include "HelperUtils.h"
#include <string>
#include <iostream>
#include "sha1.hpp"

using namespace std;
using namespace NTL;

// RSA Digital Sigature Scheme

void computePublicKey(ZZ *e, int keySize, ZZ tot_n)
{
    // Generate a psuedo random 'e' of our desired keySize.
    RandomBits(*e, keySize);

    // Makes sure e and tot_n are coprime and e is less than tot_n.
    // InvMod will fail if e > tot_n
    while (GCD(*e, tot_n) != 1 || *e > tot_n)
    {
        cout << *e << " did not work. Recomputing..."
             << "\n";
        RandomBits(*e, keySize);
    }
}

tuple<ZZ, ZZ, ZZ> generateKeys(int keySize)
{

    // Define the two primes p and q using ZZ class, n to store
    // their product, and tot_n to store the totient function result
    ZZ p, q, n, tot_n;

    // Now generate the random primes of 256 bits.
    RandomPrime(p, keySize / 2);
    RandomPrime(q, keySize / 2);

    cout << "p: " << p << "\n";
    cout << "q: " << q << "\n";
    // Calculate n and its totient. It will be difficult to calculate
    // this totient without knowing p and q.
    mul(n, p, q);
    mul(tot_n, p - 1, q - 1);
    cout << "n: " << n << "\n";
    cout << "totient(n): " << tot_n << "\n\n";

    // Now to get the public key (e) and private key (d)
    ZZ e, d;

    // Not to compute e, while making sure its inverse can be found
    // for private key (d)
    computePublicKey(&e, keySize, tot_n);

    InvMod(d, e, tot_n);

    cout << "\nThe public key(e) = " << DisplayBase64(e) << "\n";
    cout << "The private key(d) = " << DisplayBase64(d) << "\n\n";
    return {d, e, n};
}

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
    option = getchar();
    keySize = option == 'b' ? 1024 : 512;
    cout << "\nYou have chose " << keySize << " bits for the key.\n"
         << endl;

    // (d,n) will be the private key.
    // (e,n) will be the public key.
    auto [d, e, n] = generateKeys(keySize);

    // Read the message
    cout << "Enter message to encrypt: ";
    cin.ignore(100, '\n');
    getline(cin, msg_string);

    sign = RSASignatureGenerate(n, d, msg_string);
    RSASignatureVerify(e, n, msg_string, sign);
}

// ELGamal Signature Scheme

// Generate DL Parmaeters
// p = l-bits
// q = q-bits
// g <= q-1
tuple<ZZ, ZZ, ZZ> GenerateDLParameters(long l, long t)
{
    // Initialize DL Parameteres
    ZZ p, q, g;

    // Temp variable
    ZZ h, temp;

    // Computer p such that q divides p-1 using GermainPrimes
    GenGermainPrime(q, t);
    p = 2*q + 1;    

    // Generate g that is not 1
    do
    {
        RandomBnd(h, p);
        div(temp, p - 1, q);
        PowerMod(g, h, temp, p);
    } while (g == 1);

    return {p, q, g};
}

// Generates the DL Key Pair
// x is Private Key
// y is Public Key
tuple<ZZ, ZZ> GenerateDLKeyPair(ZZ p, ZZ q, ZZ g)
{
    ZZ x, y;

    RandomBnd(x, q);
    PowerMod(y, g, x, p);

    return {x, y};
}

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
        cout << "k: " << k << "\nk^-1: " << k_inverse << endl;
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

bool ElGamalSignatureVerify(ZZ p, ZZ q, ZZ g, ZZ y, ZZ r, ZZ s, string m) {
    if (r >= q || s >= q || s < 0 || r < 0){
        cout << "WARNING! Invalid Signature." << endl;
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
        cout << "Valid Signature." << endl;
        return true;
    }
    cout << "WARNING! Invalid Signature." << endl;
    return false;
}

void ElGamalDemo()
{
    string msg;

    Get the Key Size
    long keySize;
    char option;
    cout << "Select RSA Key Size \n(a) 512\n(b) 1024\n\nOption(default=a):";
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

    cout << "\nEnter message to sign: ";
    cin.ignore(100, '\n');
    getline(cin, msg);

    auto [r, s] = ElGamalSignatureGenerate(p, q, g, x, msg);
    cout << "r: " << DisplayBase64(r) << "\ns: " << DisplayBase64(s) << endl;

    ElGamalSignatureVerify(p, q, g, y, r, s, msg);
}

int main()
{
    // RSASigantureDemo();
    ElGamalDemo();
    return 0;
}