
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <stdlib.h>

#include  <bits/stdc++.h>
using namespace std;

int main() {

// Redirect cout.
streambuf* oldCoutStreamBuf = cout.rdbuf();
ostringstream strCout;
cout.rdbuf( strCout.rdbuf() );

// This goes to the string stream.
cout << "Hello, World!" << endl;

// Restore old cout.
cout.rdbuf( oldCoutStreamBuf );

// Will output our Hello World! from above.
cout << strCout.str();
}