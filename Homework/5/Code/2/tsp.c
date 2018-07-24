#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"tsp.h"
#define MAXINT 2147483647

int **adj_mat = NULL;
int **permu_table = NULL;
int thread_num, city_num, start_city, total_num, permu_gener;
pthread_t threads[20];
long int current_length = MAXINT;

void swap(int *array, int a, int b){
	
	int mid = array[a];

	array[a] = array[b];
	array[b] = mid;

}

void q_sort(int *origin, int start, int end){

	if (start < end){

		int left_mark = start, right_mark = end, mark = origin[start];

		while (left_mark < right_mark){

			while (left_mark < right_mark && origin[right_mark] >= mark) right_mark--;

			if (left_mark < right_mark){

				origin[left_mark] = origin[right_mark];

				left_mark++;
			}

			while (left_mark < right_mark && origin[left_mark] <= mark) left_mark++;

			if (left_mark < right_mark){

				origin[right_mark] = origin[left_mark];

				right_mark--;
			}

		}

	origin[left_mark] = mark;
	
	q_sort(origin, start, right_mark - 1);
	q_sort(origin, right_mark + 1, end);

	}
}

long int factorial(int n){

	if (n == 1) return 1;
	else return factorial(n - 1);

}

void *generate_route(void *args){

	int *array = args -> 

}

int main(){

	int i, j;
	long int n_fac;
	args arg;

	scanf("%d %d %d", &thread_num, &city_num, &start_city);
	/*read in every city */

	if (!(thread_num > 0 && thread_num <= 20 && start_city >= 0 && start_city < city_num)){
	/*check whether the three numbers are valid*/
		printf("-1\n");
		return 0;
	}

	adj_mat = (int**) malloc(city_num * sizeof(int*));
	/*allocate memory for adj_mat*/
	for (i = 0; i < city_num; i++){
		/*read every element*/
		adj_mat[i] = (int*) malloc(city_num * sizeof(int));
		/*allocate memeory*/
 		for (j = 0; j < city_num; j++) scanf("%d", &adj_mat[i][j]);

	}
	
	n_fac = factorial(city_num - 1);

	permu_table = (int**) malloc(n_fac * sizeof(int*));

	for (i = 0; i < n_fac; i++) permu_table[i] = (int*) malloc((city_num + 1) * sizeof(int));

	for (i = 0; i < thread_num; i++) pthread_create(&threads[i], NULL, &generate_route, &arg);
	
	for (i = 0; i < thread_num; i++) pthread_join(threads[i], NULL);

	return 0;
}