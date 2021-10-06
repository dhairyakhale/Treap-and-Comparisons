#include<bits/stdc++.h>
#include<time.h> 

#include "treap.h"
#include "avl.h"
#include "bst.h"

using namespace std;

int main()
{
	//Generate random files

	fstream tfile;

	for(int i=500;i<=10000;i+=500)
	{
		string directory = "Test_Files/"+string(to_string(i))+".txt";
		tfile.open((const char*)directory.c_str(),ios::out);

		srand(time(0));

		int j=0;
		int opi=0,opd=0;
		string op;

		for(j=1;j<=i;j++)
		{
			if(opd>(i*.3))
				tfile<<"Insert "<<randint(i)<<"\n";
			else if(opi>(i*.7))
				tfile<<"Delete "<<randint(i)<<"\n";
			else
			{
				if(randint(i)%2==0)
				{
					op = "Delete";
					opd++;
				}
				else
				{
					op = "Insert";
					opi++;
				}

				tfile<<op<<" "<<randint(i)<<"\n";
			}
		}

		tfile.close();
	}

	Treap_main();
	AVL_main();
	BST_main();

	return 0;
}