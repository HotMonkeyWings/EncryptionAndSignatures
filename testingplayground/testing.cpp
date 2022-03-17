#include <NTL/ZZ.h>
#include <csignal>
#include <tuple>

using namespace std;
using namespace NTL;

int main() {
    string s1;
    string s2;

    getline(cin, s1);

    for (auto item : s1){
        cout << item << "=" << CharToIntVal(item) << endl;

    }
}