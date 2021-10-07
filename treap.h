#include <bits/stdc++.h>
#include<time.h>

using namespace std;

int randint(int limit)
{
	return rand()%(limit+1);
}

class TreapNode
{
	private:
		TreapNode *LChild, *RChild;
		int priority;
		int key;

		TreapNode(int k, int p)
		{
			key = k;
			LChild = NULL;
			RChild = NULL;
			priority = p;
		}

		TreapNode()
		{
			key = 0;
			LChild = NULL;
			RChild = NULL;
			priority = 0;
		}

		~TreapNode()
		{
			delete LChild;
			delete RChild;
		}

	friend class Treap;
};

class Treap
{
	private:
		TreapNode *head;

		int rotation_count;
		int key_comparison;

	public:
		Treap()
		{
			head = new TreapNode();
			rotation_count = 0;
			key_comparison = 0;
		}

		void insert_Treap(int k, int p)
		{
			TreapNode *temp = head->RChild;
			TreapNode *par = head;

			stack<TreapNode*> parstk;
			parstk.push(head);

			TreapNode *t = new TreapNode(k,p);

			if(!temp)
			{
				head->RChild = t;
				return;
			}

			while(temp)
			{
				par = temp;
				parstk.push(par);

				key_comparison++;

				if(k < temp->key)
				{
					temp = temp->LChild;
				}
				else if(k > temp->key)
				{
					temp = temp->RChild;
				}
				else
				{
					delete t;
					cout<<"Element already exists."<<endl;
					return;
				}
			}

			key_comparison++;
			if(k < par->key)
				par->LChild = t;
			else
				par->RChild = t;


			while(parstk.top() != head && parstk.top()->priority > t->priority)
			{
				par = parstk.top();

				parstk.pop();

				TreapNode *p_par = parstk.top();

				rotation_count++;

				if(t == par->LChild)
				{
					par->LChild = t->RChild;
					t->RChild = par;
				}
				else if(t == par->RChild)
				{
					par->RChild = t->LChild;
					t->LChild = par;
				}

				if(p_par->LChild == par)
					p_par->LChild = t;
				else
					p_par->RChild = t;
			}
		}

		void delete_Treap(int k)
		{
			TreapNode *temp = head->RChild;
			TreapNode *par = head;

			if (!temp)
			{
				cout<<"Empty tree."<<endl;
				return;
			}
			while(temp)
			{
				key_comparison++;
				if(k < temp->key)
				{
					par = temp;
					temp = temp->LChild;
				}
				else if(k > temp->key)
				{
					par = temp;
					temp = temp->RChild;
				}
				else break;
			}
			if(!temp)
			{
				cout<<"Element not found."<<endl;
				return;
			}
			temp->priority = INT_MAX;
			
			TreapNode *t = NULL;
			TreapNode *p_par = par;
			par = temp;

			while(par->LChild != NULL || par->RChild != NULL)
			{
				if(par->LChild != NULL && par->RChild != NULL)
				{
					if(par->LChild->priority > par->RChild->priority)
						t = par->RChild;
					else
						t = par->LChild;
				}
				else
				{
					if(par->LChild != NULL)
						t = par->LChild;
					else
						t = par->RChild;
				}

				rotation_count++;

				if(t == par->LChild)
				{
					par->LChild = t->RChild;
					t->RChild = par;
				}
				else if(t == par->RChild)
				{
					par->RChild = t->LChild;
					t->LChild = par;
				}

				if(p_par->LChild == par)
					p_par->LChild = t;
				else
					p_par->RChild = t;

				p_par = t;
			}
			
			if(p_par->LChild == par)
				p_par->LChild = NULL;
			else
				p_par->RChild = NULL;

			delete par;
			return;
		}

		bool search_Treap(int k)
		{
			TreapNode *temp = head->RChild;

			while(temp)
			{
				if(k == temp->key)
				{
					return true;
				}
				if(k < temp->key)
					temp = temp->LChild;
				else
					temp = temp->RChild;
			}
			return false;
		}

		//Set labels in .gv file for all nodes uniquely
		//Helper function of print_Treap()
		void inorderLabel(TreapNode* node, fstream &myfile)
		{
			//Exit condition
			if(!node)
				return;

			inorderLabel(node->LChild, myfile);	//Recursively go to left subtree
			myfile<<"    n"<<node->key<<" [label="<<node->key<<", xlabel="<<node->priority<<"];\n";	//Assign label for node
			inorderLabel(node->RChild, myfile);	//Recursively go to right subtree
		}

		//Used to print null nodes on diagram
		//Called when node is NULL
		//Helper function of print_Treap()
		void printTree_null(int val, int nullc, fstream &myfile)
		{
		    myfile<<"    null"<<nullc<<" [shape=point];\n";
		    myfile<<"    n"<<val<<" -> null"<<nullc<<";\n";
		}
		
		//Used to print the nodes and their edges
		//Helper function of print_Treap()
		void printTree_main(TreapNode* node, fstream &myfile)
		{
		    static int nullc = 0;	//Used to name null nodes uniquely (null1, null2)
		
			//Printing all the left edges
		    if (node->LChild!=NULL)
		    {
		    	myfile<<"    n"<<node->key<<" -> n"<<node->LChild->key<<";\n";
		        printTree_main(node->LChild, myfile);		//Recursively traverse to left subtree
		    }

		    else	//Printing edge to NULL node
		        printTree_null(node->key, nullc++, myfile);
		
			//Printing all the right edges
		    if (node->RChild)
		    {
		    	myfile<<"    n"<<node->key<<" -> n"<<node->RChild->key<<";\n";
		        printTree_main(node->RChild, myfile);		//Recursively traverse to right subtree
		    }

		    else	//Printing edge to NULL node
		        printTree_null(node->key, nullc++, myfile);
		}
		
		//Driver function for printTree_main()
		void print_Treap(const char *filename)
		{
			fstream myfile;
			myfile.open("graph1.gv",ios::out|ios::trunc);	//Creating file for graph in write mode, which clears after it is opened again
			
			TreapNode* node = head->RChild;
			
			//Initial printing in .gv file
			myfile<<"digraph BST {\n";
			myfile<<"    node [fontname=\"Arial\"];\n";
			myfile<<"    graph [ordering=\"out\"];\n";
			myfile<<"    forcelabels=true;\n";
			inorderLabel(head->RChild, myfile);
			
			if (node==NULL)		//If tree is empty, graph file is empty
	        	myfile<<endl;

		    else if (node->RChild == NULL && node->LChild == NULL)		//If tree has only one node, print that
		        myfile<<"    n"<<node->key<<";\n";

		    else
	        	printTree_main(node, myfile);		//Else call printTree_main
			
			myfile<<"}\n";		//Final printing in .gv file
			
			myfile.close();		//Closing the file

			string command = "dot -Tsvg graph1.gv > "+string(filename)+".svg";
			system((const char*)command.c_str());	//Opens cmd, converts .gv into .svg, opens it

			cout<<"Find file in same directory, and open."<<endl;
		}

		int height(TreapNode *root)
		{
			if(!root)
				return 0;

			int lHeight = height(root->LChild);
			int rHeight = height(root->RChild);

			if(lHeight > rHeight)
				return (lHeight+1);
			else
				return (rHeight+1);
		}

		int fn_height()
		{
			return height(head->RChild);
		}

		void avg_height(TreapNode *root, float* ah, int *tn)
		{
			if(!root) return;

			avg_height(root->LChild, ah, tn);
			(*ah) += height(root);
			(*tn)++;
			avg_height(root->RChild, ah, tn);
		}

		float fn_avg_height()
		{
			float ah = 0;
			int tn = 0;
			avg_height(head->RChild, &ah, &tn);

			return (ah/tn);
		}

		int fn_rotation_count()
		{
			return rotation_count;
		}

		int fn_key_comparison()
		{
			return key_comparison;
		}

		~Treap()
		{
			delete head;
		}
};

void Treap_main()
{
	fstream filein, fileout;

	fileout.open("Analysis/treap_o.csv",ios::out);

	fileout<<"Treap No. of operations,Treap Height,Treap Avg height,Treap Rotation count,Treap Key comparison count\n";

	Treap *t;

	for(int i=500;i<=10000;i+=500)
	{
		t = new Treap();

		srand(time(0));

		string directory = "Test_Files/"+string(to_string(i))+".txt";
		filein.open((const char*)directory.c_str(),ios::in);

		string ch;
		int val;

		while(filein >> ch >> val)
		{
			if(ch == "Insert")
				t->insert_Treap(val, randint(100));
			else if(ch == "Delete")
				t->delete_Treap(val);
		}

		filein.close();

		fileout<<i<<","<<(t->fn_height())<<","<<to_string(t->fn_avg_height())<<","<<(t->fn_rotation_count())<<","<<(t->fn_key_comparison())<<"\n";

		delete t;
	}

	fileout.close();
}