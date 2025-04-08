#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

    int n= atoi(argv[1]);
    float *vetor1 = (float*) malloc(sizeof(float)*n);
    float *vetor2 = (float*) malloc(sizeof(float)*n);

    srand(time(NULL));  

    for(int i=0; i<n; i++){
        float a = 1.0;
        float b = 10.0;
        float r = a + ((float) rand() / RAND_MAX) * (b - a);
        vetor1[i]= r;
    }

    for(int i=0; i<n; i++){
        float a = 1.0;
        float b = 10.0;
        float r = a + ((float) rand() / RAND_MAX) * (b - a);
        vetor2[i]= r;
    }


    FILE *arquivo = fopen("resultado.bin", "wb");

    fwrite(&n, sizeof(int), 1, arquivo);
    fwrite(vetor1, sizeof(float), n, arquivo);
    fwrite(vetor2, sizeof(float), n, arquivo);
    fclose(arquivo);

    double prod=0;
    for(int i=0; i<n; i++){
        float m=vetor1[i]*vetor2[i];
        prod+=m;
    }


    arquivo = fopen("resultado.bin", "ab");
    fwrite(&prod, sizeof(double), 1, arquivo);
    fclose(arquivo);
}