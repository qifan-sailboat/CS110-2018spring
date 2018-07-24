#include"skiplist.h"
#include<iostream>
#include<cstdio>
#include<cstdlib>

using namespace std;

int main(){
	skiplist<int, int>* sl = new skiplist<int, int>(30);

	printf("sl: %p\n", sl);

	// sl->insert(2, 2);

	// sl->insert(1, 1);

	// skiplist<int, int>::iterator it=sl->insert(1, 2).first;

	// cout<<"key: "<<it->first<<" val: "<<it->second<<endl;

	// it++;

	// cout<<"key: "<<it->first<<" val: "<<it->second<<endl;

	// cout<<"erase: "<<sl->erase(0)<<endl;

	// cout<<"empty: "<<sl->empty()<<" size: "<<sl->size()<<" level: "<<sl->level()<<endl;

	// skiplist<int, int>::iterator itt=skiplist<int, int>::iterator();

	// itt++;

	for (int I = 0; I < 50; ++I)

	{

		sl->insert(I, I).second;

	}
	//cout << 2 << endl;
	//sl->Print();

	cout<<sl->insert(1, 2).second<<endl;

	cout<<sl->insert(1, 3).second<<endl;

	cout<<sl->insert(2, 5).second<<endl;

	const skiplist<int, int>* b=sl;

	skiplist<int, int>::const_iterator it=b->begin();

	for (int I = 0; I < 10; ++I)

	{

		cout<<"key: "<<it->first<<" val: "<<it->second<<endl;

		it++;

	}
	cout<<endl;

	sl->erase(37);

	while(it!=b->end()){

		cout<<"key: "<<it->first<<" val: "<<it->second<<endl;

		++it;

	}

	it = b->find(2);

	cout<<"found key: "<<(*it).first<<" val: "<<it->second<<endl;

	cout<<"empty: "<<sl->empty()<<" size: "<<sl->size()<<" level: "<<sl->level()<<endl;

	delete sl;


	return 0;
}