#include <limits>
#include <iostream>

#include <cmath>
using namespace std;

int main() {
    long a = std::numeric_limits<long>::infinity();
    cout << isinf(a);

}