#include <stdio.h>
#include <gmp.h>

typedef mpz_t big;
typedef long long ll;

/*  how to convert between mpz_t and long long */
void ll_to_mpz(ll i) {
  mpz_t a;
  mpz_init(a);
  mpz_import(a, 1, 1, sizeof(i), 0, 0, &i);
}

int main() {
  big a,b,c;
  mpz_init_set_str(a,"123456789123456789123",10);
  mpz_init_set_str(b,"98123981239457",10);
  mpz_init(c);
  mpz_mul(c,a,b);
  printf("%s * %s = %s\n",mpz_get_str(NULL,10,a),mpz_get_str(NULL,10,b),mpz_get_str(NULL,10,c));
  mpz_clear(a);
  mpz_clear(b);
  mpz_clear(c);
  return 0;
}
