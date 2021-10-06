#include <bits/stdc++.h>
#include <time.h>

#include "treap.h"

using namespace std;

int main()
{
	Treap *t = new Treap();

	int choice=0, k=0;

	srand(time(0));

	cout<<"\n1. insert\n2. delete\n3. search\n4. print tree\n5. sample tree\n0. exit\n"<<endl;

	while(1)
	{
		cout<<"Enter your option: ";
		cin>>choice;

		switch(choice)
		{
			case 0:
			{
				delete t;
				cout<<"Goodbye!"<<endl;
				return 0;
			}
			case 1:
			{
				cout<<"Enter element you want to insert: ";
				cin>>k;
				t->insert_Treap(k, randint(100));
				break;
			}
			case 2:
			{
				cout<<"Enter element you want to delete: ";
				cin>>k;
				t->delete_Treap(k);
				break;
			}
			case 3:
			{
				cout<<"Enter element you want to search: ";
				cin>>k;
				if(t->search_Treap(k))
					cout<<k<<" found!"<<endl;
				else
					cout<<k<<" not found."<<endl;

				break;
			}
			case 4:
			{
				string filename;
				cout<<"Enter name of file: ";
				cin>>filename;
				t->print_Treap((const char*)filename.c_str());
				break;
			}
			case 5:
			{
				t->insert_Treap(10,1);
				t->insert_Treap(20,5);
				t->insert_Treap(30,6);
				t->insert_Treap(40,3);
				t->insert_Treap(25,7);
				t->insert_Treap(9,8);
				t->insert_Treap(15,8);
				t->insert_Treap(35,3);
				t->insert_Treap(2,4);
				t->insert_Treap(3,2);

				t->print_Treap("sample");

				break;
			}
			default:
			{
				cout<<"Invalid input."<<endl;
				break;
			}
		}
	}

	return 0;
}