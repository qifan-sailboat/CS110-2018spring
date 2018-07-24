gcc -shared -fPIC -c skiplist.c -o liblist.so
gcc -o dynamiclist test.c -L. -llist