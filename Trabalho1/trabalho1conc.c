#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct {
    float* v1; //vetor1
    float* v2; //vetor2
    int comeco; //a partir de qual indice dos vetores a thread começa as operações
    int num_op; //quantas operações a thread vai fazer com os vetores
} t_Args;

typedef struct {
    float resultado;
} t_Ret;

void *multiplica(void* arg){

    t_Args *args = (t_Args*) arg;

    float retorno= 0;


    for(int i=0; i<args->num_op; i++){ //realiza a multiplicação das coordenadas começando pelo indice indicado em "comeco" e para quand atinge "num_op"

        retorno+=args->v1[args->comeco + i] * args->v2[args->comeco + i];
              
    }

    t_Ret *ret;
    ret = malloc(sizeof(t_Ret));
    ret->resultado=retorno;

    free(arg);

    pthread_exit((void*) ret);
}

int main(int argc, char **argv){

    t_Args *args;
    int n;
    int t = atoi(argv[1]);
    double prod_n=0;
    double prod_a;
    pthread_t *tid_sistema;
    t_Ret  *retorno;

    FILE *arquivo = fopen(argv[2], "rb");
    fread(&n, sizeof(int), 1, arquivo);

    float *vetor1 = (float*) malloc(sizeof(float)*n);
    float *vetor2 = (float*) malloc(sizeof(float)*n);

    fread(vetor1, sizeof(float), n, arquivo);
    fread(vetor2, sizeof(float), n, arquivo);
    fread(&prod_a, sizeof(double), 1, arquivo);
    
    fclose(arquivo);

    tid_sistema = (pthread_t*) malloc(sizeof(pthread_t)*t);

    int *v_ops = (int*) malloc(sizeof(int)*t);


    //Logica para balancear as threads, cada espaço do vetor v_ops represanta quantas operções a thread respespectiva vai realizar

    int num_ops_i = n/t;
    for(int i=0; i<t; i++){
        v_ops[i]=num_ops_i;
    }

    if(n>=t){
        if(n%t){
            for(int i=0; i < (n%t); i++){
                v_ops[i]+=1;
            }
        }
    } else{
        for(int i=0; i < n; i++){
            v_ops[i]+=1;
        }
    }
    
    int comeco_t= 0;

    for(int i=0; i<t; i++) {

        args = malloc(sizeof(t_Args));
        args->v1= vetor1;
        args->v2= vetor2;
        args->num_op = v_ops[i];
        args->comeco = comeco_t;
        pthread_create(&tid_sistema[i], NULL, multiplica, (void*) args);
        comeco_t += v_ops[i]; //incrementa o numero de operações realizadas para determinar a partir de onde nos vetores a proxima thread vai operar 
        
    }

    for (int i=0; i<t; i++) {

        pthread_join(tid_sistema[i], (void**) &retorno);
        prod_n += retorno->resultado;
        free(retorno);
      }

    

    double variacao_relativa = fabs((prod_a-prod_n)/(prod_a));

    printf("Produto sequencial: %.15f\n", prod_a);
    printf("Produto concorrente: %.15f\n", prod_n);
    printf("Variacao relativa: \n%.15f", variacao_relativa);

    free(vetor1);
    free(vetor2);
    free(v_ops);
    free(tid_sistema);

}


