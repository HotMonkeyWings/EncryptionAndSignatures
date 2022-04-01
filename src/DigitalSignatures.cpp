
#include <NTL/ZZ.h>
#include <csignal>
#include <tuple>
#include "HelperUtils.h"
#include <string>
#include <iostream>
#include "sha1.hpp"

using namespace std;
using namespace NTL;

// RSA Digital Sigature Scheme

void computePublicKey(ZZ* e, int keySize, ZZ tot_n) {
    // Generate a psuedo random 'e' of our desired keySize.
    RandomBits(*e, keySize);

    // Makes sure e and tot_n are coprime and e is less than tot_n.
    // InvMod will fail if e > tot_n
    while (GCD(*e, tot_n) != 1 || *e > tot_n){
        cout << *e << " did not work. Recomputing..." << "\n";
        RandomBits(*e, keySize);
    }

}

tuple<ZZ, ZZ, ZZ> generateKeys(int keySize)
{

    // Define the two primes p and q using ZZ class, n to store
    // their product, and tot_n to store the totient function result 
    ZZ p, q, n, tot_n;
    
    // Now generate the random primes of 256 bits.
    RandomPrime(p, keySize/2);
    RandomPrime(q, keySize/2);

    cout << "p: " << p << "\n";
    cout << "q: " << q << "\n";
    // Calculate n and its totient. It will be difficult to calculate
    // this totient without knowing p and q. 
    mul(n, p, q);
    mul(tot_n, p-1, q-1);
    cout << "n: " <<  n << "\n";
    cout <<"totient(n): " << tot_n << "\n\n";

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

// Private key (d, n), and message m
ZZ RSASignatureGenerate(ZZ n, ZZ d, string m) {
    string hash_hex;
    ZZ hash, sign;
    SHA1 checksum;

    // Converts message to hash in hex, then into decimal form
    checksum.update(m);
    hash_hex = checksum.final();
    hash = HexToDecimal(hash_hex);

    PowerMod(sign, hash, d, n);

    cout << "Sign is " << DisplayBase64(sign) <<endl;
    return sign;
}

bool RSASignatureVerify(ZZ e, ZZ n, string m, ZZ sign) {
    string hash_hex;
    ZZ hash, decodedHash;
    SHA1 checksum;

    // Converts message to hash in hex, then into decimal form
    checksum.update(m);
    hash_hex = checksum.final();
    hash = HexToDecimal(hash_hex);

    PowerMod(decodedHash, sign, e, n);

    if (hash == decodedHash){
        cout << "\nSignature is valid." << endl;
        return true;
    }
    cout << "\nWARNING! Signature is invalid." << endl;
    return false;
}

void RSASigantureDemo() {
    // Select Key Size
    int keySize;
    char option;
    ZZ sign;
    string msg_string;

    cout << "Select RSA Key Size \n(a) 512\n(b) 1024\n\nOption(default=a):";
    option = getchar();
    keySize = option == 'b' ? 1024 : 512;
    cout << "\nYou have chose " << keySize << " bits for the key.\n" << endl;

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

int main(int /* argc */, const char ** /* argv */)
{
    RSASigantureDemo();
    return 0;
}