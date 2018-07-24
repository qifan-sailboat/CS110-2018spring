#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"tsp.h"
#define MAXINT 2147483647
/*Include all the head filed we need*/

int **adj_mat = NULL;
/*used to store the adjcent matric*/
int *so_far_origin = NULL, *remaining_origin = NULL;
/*used to store the original array and the origin remaining array*/
int thread_num, city_num, start_city, total_num, route_len = 0, tbl_place = 0;
/*usage same as their names*/
long int current_length = MAXINT;
/*replace zero, means that their is no road their*/

void find_route(int* so_far, int* remaining, int so_far_len, int remaining_len){
	/*main working function*/
	/*list out every possible ways to go around every city*/
	
	if (so_far_len == city_num){
		/*If there is no city left for visiting*/
		int i, route_length = 0;
		/*calculate the length of each city*/

		so_far[so_far_len] = so_far[0];
		/*implement the last city*/
		/*same with the first city*/
		
		route_length = 0;
		/*calculate the length*/
		
		for (i = 0; i < so_far_len; i++){
			if (adj_mat[so_far[i]][so_far[i + 1]] != 0) route_length += adj_mat[so_far[i]][so_far[i + 1]];
			else return;
		}
		/*from the first city to the last, calculate the length of the route*/

		if (route_length < current_length) current_length = route_length;
		/*if the route is shorter than provious ones*/
		/*update it as the shortest one*/

	}else{
		/*it means that there is still some cities left*/
		int i;

		/*printf("%d %d\n", so_far_len, remaining_len);*/
		for (i = 0; i < city_num; i++){
			/*search one by one*/
			/*if remaining[i] == -1, it means we have visited this city*/
			/*if remaining[i] != -1, it means the city has not been visited*/
			if (remaining[i] != -1){
			/*if the city has not been visited*/
				int *so_far_next = NULL;
				/*copy the so_far list and change the one we visit this time*/
				int *remaining_next = NULL;
				/*copy the remaining list and delete the one we visit this time*/
				int j;

				so_far_next = (int*) malloc((city_num + 1) * sizeof(int));
				/*arrange size for next so_far array*/
				remaining_next = (int*) malloc(city_num * sizeof(int));
				/*arrange memeory for next remaining array*/

				for (j = 0; j < so_far_len; j++) so_far_next[j] = so_far[j];
				/*copy the so_far array*/
				so_far_next[so_far_len] = i;
				/*add the next one in the so_far array*/

				for (j = 0; j < city_num; j++) remaining_next[j] = remaining[j];
				/*copy the remaining array*/
				remaining_next[i] = -1;
				/*mark this city as visited*/

				find_route(so_far_next, remaining_next, so_far_len + 1, remaining_len - 1);
				/*find the next city we want to visit*/

				free(so_far_next);
				/*if ended, free the memory allocated to so_far_next*/
				free(remaining_next);
				/*if ended, free the memory allocated to remaining_next*/
			}
		}
	}
}

void free_all(){
	/*it is used to free all the memories used*/
	int i;

	for (i = 0; i < city_num; i++) free(adj_mat[i]);
	/*free every line of adjcent matrix*/
	free(adj_mat);
	/*free the whole matrix*/
	free(so_far_origin);
	/*free the so_far_origin*/
	free(remaining_origin);
	/*free the remaining_origin*/
}

int main(){

	int i, j;

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
 		for (j = 0; j < city_num; j++){
			int mid;

			scanf("%d", &mid);

			/*if (mid == 0) mid = MAXINT;*/
			/*if the element is 0*/
			/*it means that there is no road there*/
			/*set it as MAXINT*/

			adj_mat[i][j] = mid;
			/*give the weigh of the road*/
		}
	}

	so_far_origin = (int*) malloc((city_num + 1) * sizeof(int));
	/*allocate memory to so_far_origin*/
	remaining_origin = (int*) malloc(city_num * sizeof(int));
	/*allocate memory to remaining_origin*/
	
	so_far_origin[0] = start_city;
	/*mark the first city as the start city*/
	for (i = 0; i < city_num; i++) remaining_origin[i] = 7;
	/*mark every city as unvisited*/

	remaining_origin[start_city] = -1;
	/*except the first one*/
	
	find_route(so_far_origin, remaining_origin, 1, city_num - 1);
	/*start finding routes*/

	free_all();
	/*Free all the memories allocated*/	

	if (current_length < MAXINT) printf("%ld\n", current_length);
	else printf("-1\n");
	/*print the result*/

	return 0;
}