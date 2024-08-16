#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 200
#define BUF_SIZE 64

int iterations_div_by_max_threads = 5000000;

int para_chance(void * rand_arg) {
    int max_para = 0;
    int iter = 0;
    int result;

    //printf("hi from thread %ld\n", pthread_self());

    while (max_para < 177 && iter < iterations_div_by_max_threads) {
        int para = 0;
        for (int i = 0; i < 232; i++) {
            random_r((struct random_data *) rand_arg, &result);
            //printf("result: %d", (result % 4));
            if ((result % 4) == 0) {
                para++;
            }
        }

        if (para > max_para) {
            max_para = para;
        }

        iter++;
    }

    //printf("iterations: %d\n", iter);
    //printf("iteration address: %p\n", &iter);

    //printf("MAX para: %d\n", max_para);
    //sleep(30);
    return max_para;
}

int main() {
    pthread_t tid[MAX_THREADS];
    int possible_max[MAX_THREADS];
    int max_num = 0;
    struct random_data *rand_states = (struct random_data *) calloc(MAX_THREADS * 64, sizeof(struct random_data));
    char *rand_statebufs = (char *) calloc(MAX_THREADS * 64, BUF_SIZE);

    for (int i = 0; i < MAX_THREADS; i++) {
        initstate_r(random(), &(rand_statebufs[i*64]), BUF_SIZE, &(rand_states[i*64]));
        pthread_create(&(tid[i]), NULL, (void *) &para_chance, &(rand_states[i*64]));
        //pthread_join(tid[i], (void *) &(possible_max[i]));
    }

    for (int j = 0; j < MAX_THREADS; j++) {
        pthread_join(tid[j], (void *) &(possible_max[j]));
    }
    
    free(rand_states);
    free(rand_statebufs);

    for (int k = 0; k < MAX_THREADS; k++) {
        //printf("aaa: %d", possible_max[k]);
        if (possible_max[k] > max_num) {
            max_num = possible_max[k];
        }    
    }
    
    printf("Highest Paralysis Rolls: %d\n", max_num);
    printf("Number of Attempts: %d\n", iterations_div_by_max_threads * MAX_THREADS);

    return 0;
}