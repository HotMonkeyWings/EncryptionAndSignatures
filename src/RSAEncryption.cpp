#include <NTL/ZZ.h>
#include <csignal>
#include <tuple>
#include "HelperUtils.h"

using namespace std;
using namespace NTL;

void computePublicKey(ZZ* e, int keySize, ZZ tot_n) {
    // Generate a psuedo random 'e' of our desired keySize.
    // Keep iterating until the GCD of it and totient is 1
    RandomBits(*e, keySize);
    while (GCD(*e, tot_n) != 1){
        cout << *e << " did not work. Recomputing..." << "\n";
        RandomBits(*e, keySize);
    }

}
// A signal handler for big functions
void signal_handle(int sig){
    cout << "The random e was too big to handle. Please re-run the program.";
    exit(sig);
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
    recompute: computePublicKey(&e, keySize, tot_n);

    InvMod(d, e, tot_n);


    cout << "\nThe public key(e) = " << e << "\n";
    cout << "The private key(d) = " << d << "\n\n";
    return {d, e, n};
}

int main() {
    // Using a signal handler for big numbers
    signal(SIGABRT, signal_handle);

    int keySize;
    cout << "Enter RSA Key Size: ";
    cin >> keySize;

    // (d,n) will be the private key.
    // (e,n) will be the public key.
    auto [d, e, n] = generateKeys(keySize);

    // Defining message and its encryption
    // and a test output for decryption
    ZZ msg_raw, msg_encrypted, msg_decrypted;
    string msg_string;

    // A temp variable for storing long vars
    long temp;

    // The rest here is self explanatatory. 
    cout << "Enter word to encrypt: ";
    cin.ignore(100, '\n');
    getline(cin, msg_string);
    msg_raw = EncodeToLong(msg_string);

    PowerMod(msg_encrypted, msg_raw, e, n);

    cout << "\nEncrypted message is " << msg_encrypted << "\n";
    cout << "\nDecrypting...\n";
    
    PowerMod(msg_decrypted, msg_encrypted, d, n);
    conv(temp, msg_decrypted);

    msg_string = DecodeToText(temp);

    cout << "Decrypted message is " << msg_string << "\n";

}