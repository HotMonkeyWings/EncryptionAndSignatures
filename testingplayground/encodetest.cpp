// C++ program to encode an ASCII
// string in Base64 format
#include <iostream>
using namespace std;
#define SIZE 1000

// Takes string to be encoded as input
// and its length and returns encoded string
char *base64Encoder(char input_str[], int len_str)
{

    // Character set of base64 encoding scheme
    char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    // Resultant string
    char *res_str = (char *)malloc(SIZE * sizeof(char));

    int index, no_of_bits = 0, padding = 0, val = 0, count = 0, temp;
    int i, j, k = 0;

    // Loop takes 3 characters at a time from
    // input_str and stores it in val
    for (i = 0; i < len_str; i += 3) {
        val = 0, count = 0, no_of_bits = 0;

        for (j = i; j < len_str && j <= i + 2; j++)
        {
            // binary data of input_str is stored in val
            val = val << 8;

            // (A + 0 = A) stores character in val
            val = val | input_str[j];

            // calculates how many time loop
            // ran if "MEN" -> 3 otherwise "ON" -> 2
            count++;
        }

        no_of_bits = count * 8;

        // calculates how many "=" to append after res_str.
        padding = no_of_bits % 3;

        // extracts all bits from val (6 at a time)
        // and find the value of each block
        while (no_of_bits != 0)
        {
            // retrieve the value of each block
            if (no_of_bits >= 6)
            {
                temp = no_of_bits - 6;

                // binary of 63 is (111111) f
                index = (val >> temp) & 63;
                no_of_bits -= 6;
            }
            else
            {
                temp = 6 - no_of_bits;

                // append zeros to right if bits are less than 6
                index = (val << temp) & 63;
                no_of_bits = 0;
            }
            cout << input_str[i] << ":" << index << ":" << char_set[index] << "\n";
            res_str[k++] = char_set[index];
        }
    }

    // padding is done here
    for (i = 1; i <= padding; i++)
    {
        res_str[k++] = '=';
    }

    res_str[k] = '\0';

    return res_str;
}

// Driver code
int main()
{
    char input_str[] = "MENON ";
    int len_str;

    // calculates length of string
    len_str = sizeof(input_str) / sizeof(input_str[0]);

    // to exclude '\0' character
    len_str -= 1;

    cout << "Input string is : " << input_str << endl;
    cout << base64Encoder(input_str, len_str) << endl;
    return 0;
}

// This code is contributed by shivanisinghss2110
