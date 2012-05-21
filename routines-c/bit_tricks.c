/* http://graphics.stanford.edu/~seander/bithacks.html */
// CHAR_BIT is the number of bits per byte (normally 8).

int v;           // we want to find the absolute value of v
unsigned int r;  // the result goes here 
int const mask = v >> sizeof(int) * CHAR_BIT - 1;

r = (v + mask) ^ mask;

r = (v ^ mask) - mask;

/************************************/

int x;  // we want to find the minimum of x and y
int y;   
int r;  // the result goes here 

r = y ^ ((x ^ y) & -(x < y)); // min(x, y)

