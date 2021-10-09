#include <bits/stdc++.h>

using namespace std;

class BST_Node
{
	private:
		int key;
		BST_Node *LChild, *RChild;

		BST_Node(int k)
		{
			key = k;
			LChild = NULL;
			RChild = NULL;
		}

		BST_Node()
		{
			key = 0;
			LChild = NULL;
			RChild = NULL;
		}

		~BST_Node()
		{
			delete LChild;
			delete RChild;
		}

	friend class BST;
};

class BST
{
	private:
		BST_Node *head;

		int rotation_count;
		int key_comparison;

	public:
		BST()
		{
			head = new BST_Node();
			rotation_count = 0;
			key_comparison = 0;
		}

		//Inserting into a BST
		void BST_Insert(int k)
		{
			BST_Node *node = head->RChild;
			BST_Node *par = head;

			BST_Node *temp = new BST_Node(k);

			//If tree empty, make new node as root
			if(!node)
			{
				head->RChild = temp;
				return;
			}

			//Traverse into tree
			while(node)
			{
				key_comparison++;

				par = node;

				if(k < node->key)			//Go to left subtree
					node = node->LChild;

				else if(k > node->key)		//Go to right subtree
					node = node->RChild;

				else						//Node already exists
				{
					delete temp;
					cout<<"Node already exists."<<endl;
					return;
				}
			}

			key_comparison++;

			if(k<par->key)				//Enter as left child (as a leaf)
				par->LChild = temp;

			else						//Enter as right child (as a leaf)
				par->RChild = temp;
		}

		//Deleting node with no children (leaf)
		//Helper function of BST_Delete()
		void Del0Child(BST_Node* q, BST_Node* par)
		{
			key_comparison++;
			if(q->key < par->key)		//If q is left child of parent
				par->LChild = NULL;

			else						//If q is right child of parent
				par->RChild = NULL;

			delete q;
		}

		//Deleting node with 1 child
		//Helper function of BST_Delete()
		void Del1Child(BST_Node* q, BST_Node* par)
		{
			if(!q->LChild)				//If q has only right child
			{
				//Connecting child of q to parent

				key_comparison++;
				if(q->key < par->key)			//If q is left child of parent
					par->LChild = q->RChild;
				else							//If q is right child of parent
					par->RChild = q->RChild;

				q->RChild = NULL;	//Detach q
			}

			else	//If q has only left child
			{
				//Connecting child of q to parent
				//Same as above

				key_comparison++;
				if(q->key < par->key)
					par->LChild = q->LChild;
				else
					par->RChild = q->LChild;

				q->LChild = NULL;
			}

			delete q;
		}

		//Function to delete node with value k
		void BST_Delete(int k)
		{
			BST_Node* par = head;
			BST_Node* p = head->RChild;

			//Locating element to be deleted
			while(p)
			{
				key_comparison++;
				if(k < p->key)			//Traverse into left subtree
				{
					par = p;
					p = p->LChild;
				}

				else if(k > p->key)		//Traverse into right subtree
				{
					par = p;
					p = p->RChild;
				}

				else					//Element found
					break;
			}

			if(!p)						//If element not found or tree is empty
			{
				cout<<k<<" not found for deletion."<<endl;
				return;
			}

			//Deletion of the element

			if(!p->LChild && !p->RChild)	//If element to be deleted is a leaf	
			{
				Del0Child(p, par);
			}

			else if(!p->LChild || !p->RChild)	//If element to be deleted has only 1 child
			{
				Del1Child(p, par);
			}

			else	//If element to be deleted has 2 children
			{
				BST_Node *inSucc = p->RChild;

				while(inSucc->LChild)			//Locating inorder successor of element
				{
					par = inSucc;
					inSucc = inSucc->LChild;
				}

				p->key = inSucc->key;			//Copying inorder successor's key to the 2-degree node

				k = inSucc->key;				//Saving it in k

				if(!inSucc->LChild && !inSucc->RChild)	//If inorder successor is leaf
				{
					Del0Child(inSucc, par);
				}
				else									//If inorder successor has 1 child
				{
					Del1Child(inSucc, par);
				}
			}
		}

		//Comments for below same as file treap.h

		int height(BST_Node *root)
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

		void avg_height(BST_Node *root, float* ah, int *tn)
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

		~BST()
		{
			delete head;
		}
};

void BST_main()
{
	fstream filein, fileout;

	fileout.open("Analysis/bst_o.csv",ios::out);

	fileout<<"BST No. of operations,BST Height,BST Avg height,BST Rotation count,BST Key comparison count\n";

	BST *t;

	for(int i=500;i<=10000;i+=500)
	{
		t = new BST();

		srand(time(0));

		string directory = "Test_Files/"+string(to_string(i))+".txt";
		filein.open((const char*)directory.c_str(),ios::in);

		string ch;
		int val;

		while(filein >> ch >> val)
		{
			if(ch == "Insert")
				t->BST_Insert(val);
			else if(ch == "Delete")
				t->BST_Delete(val);
		}

		filein.close();

		fileout<<i<<","<<(t->fn_height())<<","<<to_string(t->fn_avg_height())<<","<<(t->fn_rotation_count())<<","<<(t->fn_key_comparison())<<"\n";

		delete t;
	}

	fileout.close();
}