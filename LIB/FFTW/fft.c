#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>
#include <math.h>

#define PI 3.14159265358979323846

// Timing function
// rdtcs returns number of processor cycles
static unsigned long long rdtsctime() {
    unsigned int eax, edx;
    unsigned long long val;
    __asm__ __volatile__("rdtsc":"=a"(eax), "=d"(edx));
    val = edx;
    val = val << 32;
    val += eax;
    return val;
}

void my_fft(complex double * in, complex double * out, int n){
    // TODO: Enter your FFT code here
}


// Naive discreete Fourier transform
// Time complexity: O(n^2)
void dft_naive(complex double * in, complex double * out, int n){
    for(int k = 0; k < n; k++){
        out[k] = 0 + 0*I;
        for(int j = 0; j < n; j++){
            complex double t = cos(-2*PI*j*k/n) + sin(-2*PI*j*k/n) * I;
            out[k] += in[j]*t;
        }
    }
}


// Returns a new array with the elements of in with even indices
complex double* get_even(complex double* in, int N){
    complex double* out = (complex double*)malloc(sizeof(complex double) * N/2);
    for(int i = 0; i < N/2; i++){
        out[i] = in[2*i];
    }
    return out;
}


// Returns a new array with the elements of in with odd indices
complex double* get_odd(complex double* in, int N){
    complex double* out = (complex double*)malloc(sizeof(complex double) * N/2);
    for(int i = 0; i < N/2; i++){
        out[i] = in[2*i + 1];
    }
    return out;
}


// Naive fast Fourier transform
// Time complexity: O(n log n)
void fft_naive(complex double * in, complex double * out, int N){

    if(N < 1)
        return;

    if(N == 1){
        out[0] = in[0];
    }

    //divide
    complex double* even = get_even(in, N);
    complex double* odd = get_odd(in, N);

    complex double* even_out = (complex double*)malloc(sizeof(complex double)*N/2);
    complex double* odd_out = (complex double*)malloc(sizeof(complex double)*N/2);

    //conquer
    fft_naive(even, even_out, N/2);
    fft_naive(odd, odd_out, N/2);

    //combine
    for(int k = 0; k < N/2; k++){
        complex double t = cos(-2*PI*k/N) + I * sin(-2*PI*k/N);
        t = t * odd_out[k];
        out[k] = even_out[k] + t;
        out[k + N/2] = even_out[k] - t;
    }

    free(even);free(odd);free(even_out);free(odd_out);
}

int main(int argc, char** argv){

    // Error message for wrong arguments
    if(argc<3) {
        puts("Usage: fft size algo\n");
        puts("size is the size of the array to transform, it must be a power of two.");
        puts("algo: select which algorithm to run.");
        puts("      1 - naive dft");
        puts("      2 - naive fft");
        puts("      3 - your code");
        puts("      4 - fftw");
        puts("Example: fft 1024 3");
        return 0;
    }

    // Parsing command line arguments
    int size=strtol(argv[1],0,10);
    int algo=strtol(argv[2],0,10);
    int log2=-1;
    for(int i=0;i<31;i++) if(size==(1<<i)) log2=i;
    if(log2==-1) {
        printf("size must be a power of two!\n");
        return 0;
    }
    if(algo<1 || algo>4) {
        puts("algo must be between 1 and 4.");
        return 0;
    }

    // Approximate number of floating point opperations for the given size
    // (Assuming fft like algorithm)
    double flop = 5*size*log2;

    // Timing variables
    unsigned long long start,stop,cycles;

    // Input/output buffers
    complex double* in = (complex double*)malloc(sizeof(complex double)*size);
    complex double* out = (complex double*)malloc(sizeof(complex double)*size);
    complex double* out_fftw = (complex double*)malloc(sizeof(complex double)*size);

    // Creating fftw plan
    fftw_plan p;
    p = fftw_plan_dft_1d(size, in, out_fftw, FFTW_FORWARD, FFTW_MEASURE);

    // Filling input array
    for(int i = 0; i < size; i++){
        in[i] = pow(2.718281828,(double)i/size)+sin(i*i) +
            I*(pow((1+sqrt(5))/2,-(double)i/size)+cos(i*i));
    }

    // Running and timing selected algorithm
    switch(algo) {
        case 1:
            start = rdtsctime();
            dft_naive(in, out, size);
            stop = rdtsctime();
            cycles = stop - start;
            printf ( "dft_naive took %.4f flop per 1000 cycles\n", 1000*flop / cycles );
            break;
        case 2:
            start = rdtsctime();
            fft_naive(in, out, size);
            stop = rdtsctime();
            cycles = stop - start;
            printf ( "fft_naive took %.4f flop per 1000 cycles\n", 1000*flop / cycles );
            break;
        case 3:
            start = rdtsctime();
            my_fft(in, out, size);
            stop = rdtsctime();
            cycles = stop - start;
            printf ( "my_fft took %.4f flop per 1000 cycles\n", 1000*flop / cycles );
            break;
        case 4:
            start = rdtsctime();
            fftw_execute(p);
            stop = rdtsctime();
            cycles = stop - start;
            printf ( "fftw took %.4f flop per 1000 cycles\n", 1000*flop / cycles );
    }

    // Comparing output with fftw to check for correctness
    if(algo!=4) {
        fftw_execute(p);

        int errors=0;
        for(int i = 0; i < size; i++){
            complex double diff = out_fftw[i] - out[i];
            if(fabs(creal(diff))>1e-8 || fabs(cimag(diff))>1e-8) {
                errors++;
                if(errors<10) {
                    printf("Error at %d, correct is (%f %f), my_fft returned (%f %f)\n",
                            i,creal(out_fftw[i]),cimag(out_fftw[i]),creal(out[i]),cimag(out[i]));
                } else if(errors==10) puts("...");
            }
        }
        printf("number of errors: %d\n",errors);
    }

    free(in);
    free(out);
    free(out_fftw);

    return 0;
}
