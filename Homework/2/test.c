#include "skiplist.c"
#include <stdio.h>

void printList(list l) { /* print list horizontally */

	int i;
	node n;
	for (i = l->level - 1; i >= 0; --i)
	{
		n = l->header;
		printf("\n%d: NULL ", i);
		while (n->forward[i]) {
			n = n->forward[i];
			printf("-> %d|%d ", *(int *)n->key, *(int *)n->val);
		}
	}
	printf("\n");
}

void printListVer(list l) { /* print list vertically with address */
	int i;
	node n = l->header;
	while (n) {
		printf("%p: ", (void *)n);
		for (i = 0; i < l->level; ++i)
		{
			printf("%p,", (void *)n->forward[i]);
		}
		n = n->forward[0];
		printf("\n");
	}
}

int main()
{
	list ll = createSkipList(10, &compareInt, &*allocInt, &deleteInt);
	int i, j, k, flag;
	int * ip;
	char c;
	flag = 1;

	/* insert some node with rand */
	for (i = 0; i < 10; ++i)
	{
		j = rand();
		insertNode(ll, &j, &i);
	}

	printList(ll);

	printf("'i' for insert, 'd' for delete, 's' for search, 'f' for free, 'p' for print, 'v' for print vertically with address, 'q' to quit\n");
	setbuf(stdin, NULL);

	while (flag && scanf("%c%*c", &c)) {
		switch (c) {
		case 'i':
			printf("the key you want\n");
			setbuf(stdin, NULL);
			if (scanf("%d", &i)) {
				printf("the val you want\n");
				setbuf(stdin, NULL);
				if (scanf("%d", &j)) {
					switch (insertNode(ll, &i, &j)) {
					case 1:
						printf("key already exist\n");
						break;
					case 0:
						printf("insert successfully\n");
						break;
					case -1:
						printf("insert failed\n");
						break;
					default:
						printf("unknow error, check your source code please\n");
						break;
					}
				}
			}
			break;
		case 'd':
			printf("the key you want:\n");
			setbuf(stdin, NULL);
			if (scanf("%d", &k))
			{
				j = deleteNode(ll, &k);
				if (j == 0)
					printf("delete successfully\n");
				else if (j == -1)
					printf("delete failed\n");
			}
			break;
		case 's':
			printf("the key you want:\n");
			setbuf(stdin, NULL);
			if (scanf("%d", &k)) {
				ip = searchNode(ll, &k);
				if (ip) {
					printf("this node has value %d\n", *ip);
				}
				else {
					printf("Not found\n");
				}
			}
			break;
		case 'f':
			freeSkipList(ll);
			break;
		case 'p':
			printList(ll);
			break;
		case 'v':
			printListVer(ll);
			break;
		case 'q':
			freeSkipList(ll);
			flag = 0;
			break;
		default :
			printf("invalid input, please enter ");
			break;
		}
		if (flag) printf("i/d/s/f/p/v/q:");
		setbuf(stdin, NULL);
	}
	return 0;
}
