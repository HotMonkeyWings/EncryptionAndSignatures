#include <NTL/ZZ.h>

#include "../utils/EncodeUtils.hpp"
#include "../utils/RSAUtils.hpp"

using namespace std;
using namespace NTL;

int main() {
    int keySize;
    char option;
    cout << "Select RSA Key Size \n(a) 512\n(b) 1024\n\nOption(default=a):";
    option = getchar();
    keySize = option == 'b' ? 1024 : 512;
    cout << "\nYou have chose " << keySize << " bits for the key.\n" << endl;

    // (d,n) will be the private key.
    // (e,n) will be the public key.
    auto [d, e, n] = GenerateRSAKeys(keySize);

    // Defining message and its encryption
    // and a test output for decryption
    ZZ msg_raw, msg_encrypted, msg_decrypted;
    string msg_string, display_string;

    // A temp variable for storing long vars
    long temp;

    // The rest here is self explanatatory. 
    cout << "Enter message to encrypt: ";
    cin.ignore(100, '\n');
    getline(cin, msg_string);
    msg_raw = Encode(msg_string);

    PowerMod(msg_encrypted, msg_raw, e, n);

    cout << "\nEncrypted message is " << DisplayBase64(msg_encrypted) << "\n";
    cout << "\nDecrypting...\n";
    
    PowerMod(msg_decrypted, msg_encrypted, d, n);

    cout << "Decrypted message is: " << Decode(msg_decrypted) << "\n\n";

    // An NTL Error happens here, I'm not sure why. But it doesn't
    // Affect the code so I'm going to leave it there for now.
}
