#include <gmpxx.h>
#include <iostream>
using namespace std;

int main() {
  mpz_class a,b;
  a="324323432483248324745435435435435893274789";
  b="-874843684756875643875684375765675643";
  cout << (a+b) << endl;
  cout << (a-b) << endl;
  cout << (a*b) << endl;
  cout << (a/b) << endl;
  cout << (a%b) << endl;
  return 0;
}
