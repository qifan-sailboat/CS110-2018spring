#ifndef TSP_H
#define TSP_H

typedef struct arg_node
{
	int* so_far;
	int* remaining;
	int so_far_len;
	int remaining_len;
}arg_node;

#endif