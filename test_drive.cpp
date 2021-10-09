#include<bits/stdc++.h>
#include<time.h> 

//Files included for running tree-specific functions
#include "treap.h"
#include "avl.h"
#include "bst.h"

using namespace std;

int main()
{
	//Generate random files

	fstream tfile;

	for(int i=500;i<=10000;i+=500)	//Generating files of rows 500 to 10000, with step of 500
	{
		string directory = "Test_Files/"+string(to_string(i))+".txt";	//File name is 500.txt for a file of 500 rows
		tfile.open((const char*)directory.c_str(),ios::out);

		srand(time(0));	//Seed for random input

		int j=0;			//Iterator
		int opi=0,opd=0;	//Count for "Insert" and "Delete" operations

		string op;

		for(j=1;j<=i;j++)	//Loop in no. of rows
		{
			if(opd>(i*.3))								//If delete is more than 30%, put "Insert"
				tfile<<"Insert "<<randint(i)<<"\n";

			else if(opi>(i*.7))							//If insert is more than 30%, put "Delete"
				tfile<<"Delete "<<randint(i)<<"\n";

			else
			{
				//Randomly deciding to delete or insert
				if(randint(i)%3==0)
				{
					op = "Delete";
					opd++;				//Incrementing delete count
				}

				else
				{
					op = "Insert";
					opi++;				//Incrementing insert count
				}

				tfile<<op<<" "<<randint(i)<<"\n";	//Writing into test file
			}
		}

		tfile.close();
	}

	//Calling tree-specific functions using the created test files
	Treap_main();
	AVL_main();
	BST_main();

	return 0;
}