#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

void GenerateRSAPublicKey(ZZ* e, int keySize, ZZ tot_n) {
    // Generate a psuedo random 'e' of our desired keySize.
    RandomBits(*e, keySize);

    // Makes sure e and tot_n are coprime and e is less than tot_n.
    // InvMod will fail if e > tot_n
    while (GCD(*e, tot_n) != 1 || *e > tot_n){
        cout << "Public Key(e): "<< *e << " did not work. Recomputing..." << "\n";
        RandomBits(*e, keySize);
    }

}

tuple<ZZ, ZZ, ZZ> GenerateRSAKeys(int keySize)
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
    GenerateRSAPublicKey(&e, keySize, tot_n);

    InvMod(d, e, tot_n);


    cout << "\nThe public key(e) = " << DisplayBase64(e) << "\n";
    cout << "The private key(d) = " << DisplayBase64(d) << "\n\n";
    return {d, e, n};
}