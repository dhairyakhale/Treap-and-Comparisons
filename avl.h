#include<bits/stdc++.h>

using namespace std;

//Used to create a node for AVL (self-balancing) BST
class AVL_Node
{
	private:

		int key;
		int bf;		//Balance factor = height(left subtree) - height(right subtree)
					//However, we aren't allowed to calculate it

		AVL_Node *LChild, *RChild;

		//Initializing AVL node with key k
		AVL_Node(int k)
		{
		 	key = k;
		 	LChild = NULL;
		 	RChild = NULL;
		 	bf = 0;
		}

		//Initializing empty AVL node
		AVL_Node()
		{
		 	key = 0;
		 	LChild = NULL;
		 	RChild = NULL;
		 	bf = 0;
		}

		//Destructor to free the node
		~AVL_Node()
		{
			delete LChild;
			delete RChild;
		}

		//Associating AVL_Tree class to this class
	friend class AVL_Tree;
};

//Used to create tree using AVL nodes defined above
//Also contains functions and operations on tree
class AVL_Tree
{
	private:

		AVL_Node* head;		//Pointer to dummy node. head->RChild = root of the tree

		int rotation_count;
		int key_comparison;

	public:

		//Initializing an AVL tree
		AVL_Tree()
		{
			head = new AVL_Node();	//Dummy node created
			head->LChild = NULL;
			head->RChild = NULL;

			rotation_count = 0;
			key_comparison = 0;
		}

		//Resulting from L-L or R-R imbalance
		int singleRotation(int a, AVL_Node **p, AVL_Node **r, AVL_Node **s)
		{
			(*p) = (*r);	//p will be the root node of the subtree resulting from rotation

			//L-L imbalance, results Right rotation
			if(a == 1)
			{
				(*s)->LChild = (*r)->RChild;
				(*r)->RChild = (*s);
			}

			//R-R imbalance, results Left rotation
			else if (a == -1)
			{
				(*s)->RChild = (*r)->LChild;
				(*r)->LChild = (*s);
			}

			//Balance factors of left and right nodes after rotation is zero
			(*r)->bf = 0;
			(*s)->bf = 0;

			return 1;
		}

		//Resulting from R-L or L-R imbalance
		int doubleRotation(int a, AVL_Node **p, AVL_Node **r, AVL_Node **s)
		{
			//p will be the root node of the subtree resulting from rotation

			//L-R imbalance, results Left-Right rotation
			if(a == 1)
			{
				(*p) = (*r)->RChild;
				(*r)->RChild = (*p)->LChild;
				(*p)->LChild = (*r);
				(*s)->LChild = (*p)->RChild;
				(*p)->RChild = (*s);
			}

			//R-L imbalance, results Right-Left rotation
			else if (a == -1)
			{
				(*p) = (*r)->LChild;
				(*r)->LChild = (*p)->RChild;
				(*p)->RChild = (*r);
				(*s)->RChild = (*p)->LChild;
				(*p)->LChild = (*s);
			}

			//Adjustment of balance factors

			if((*p)->bf == a)	//If root has bf=a
			{
				(*s)->bf = -1*a;
				(*r)->bf = 0;
			}
			else if((*p)->bf == 0)	//If root has bf=0
			{
				//Both its children will also be balanced
				(*s)->bf = 0;
				(*r)->bf = 0;
			}
			else if((*p)->bf == (-1*a))		//If root has bf = -a
			{
				(*s)->bf = 0;
				(*r)->bf = a;
			}

			(*p)->bf = 0;

			return 2;
		}
		
		//Inserting a new node into AVL tree with key of node as k
		void AVL_Insert(int k)
		{
			AVL_Node* t = head;				//Parent of s
			AVL_Node* s = head->RChild;		//Point of imbalance
			AVL_Node* p = head->RChild;		//Node which will travel through tree, also parent of q

			AVL_Node *q = NULL, *r = NULL;	//q will be location of insertion, r will be relevant child of point of imbalance

			AVL_Node* temp = new AVL_Node(k);	//temp = new AVL node which is to be inserted
			
			//Step 1: Insertion of node

			if(p==NULL)						//If tree is empty
			{
				head->RChild = temp;		//New node becomes root
				return;
			}

			else
			{
				while(1)
				{
					key_comparison++;

					//Traversal into left subtree
					if(k < p->key)
					{
						q = p->LChild;

						if(!q)	//If location to insert is reached
						{
							q = temp;
							p->LChild = q;
							break;
						}

						else
						{
							if(q->bf)		//If balance factor of q is non-zero, it becomes possible point of imbalance
							{
								t = p;		
								s = q;
							}
							p = q;
						}
					}

					//Traversal into right subtree
					else if(k > p->key)
					{
						q = p->RChild;

						if(!q)
						{
							q = temp;
							p->RChild = q;
							break;
						}

						else
						{
							if(q->bf)
							{
								t = p;
								s = q;
							}
							p = q;
						}
					}

					//Element already present
					else
					{
						delete temp;
						cout<<k<<" already present."<<endl;
						return;
					}
				}

				//Step 2: Adjustment of balance factors

				int a;

				key_comparison++;
				if(k < s->key)		//If new element inserted in LST
					a = 1;
				else
					a = -1;			//If new element inserted in RST


					//Changing bf from below the point of imbalance to inserted node
				if(a == 1)
					p = s->LChild;
				else
					p = s->RChild;

				r = p;	//r will be the child of s in subtree of inserted node


					//For each p, if new element is in its LST, bf = 1. If RST, bf = -1
				while(p != q)
				{
					key_comparison++;
					if(k < p->key)
					{
						p->bf = 1;
						p = p->LChild;
					}

					else if(k > p->key)
					{
						p->bf = -1;
						p = p->RChild;
					}
					else break;
				}

				//Step 3: Rotation

					//If bf of point of imbalance was 0, it will become a
					//This is due to new insertion into either its LST or RST
				if(s->bf == 0)
				{
					s->bf = a;
					return;
				}

					//If bf of point of imbalance was -a, it will become 0
					//This is due to new insertion now balancing the tree
				else if(s->bf == (-1*a))
				{
					s->bf = 0;
					return;
				}

				else if(s->bf == a)
				{
					//L-L or R-R imbalance
					if(r->bf == a)
					{
						rotation_count += singleRotation(a, &p, &r, &s);
					}

					//L-R or R-L imbalance
					else if(r->bf == (-1*a))
					{
						rotation_count += doubleRotation(a, &p, &r, &s);
					}
				}

					//Since s is removed from original location because of rotation
					//p is in old location of s
				if (s == t->RChild)
					t->RChild = p;
				else
					t->LChild = p;
			}
		}

		//Deleting node with no children (leaf)
		//Helper function of AVL_Delete()
		void Del0Child(AVL_Node* q, AVL_Node* par)
		{
			key_comparison++;
			if(q->key < par->key)		//If q is left child of parent
				par->LChild = NULL;

			else						//If q is right child of parent
				par->RChild = NULL;

			delete q;
		}

		//Deleting node with 1 child
		//Helper function of AVL_Delete()
		void Del1Child(AVL_Node* q, AVL_Node* par)
		{

			if(!q->LChild)				//If q has only right child
			{
				//Connecting child of q to parent

				par->bf = q->RChild->bf;	//Copying imbalance of new child to parent

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

				par->bf = q->LChild->bf;

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
		void AVL_Delete(int k)
		{
			AVL_Node* t = head;
			AVL_Node* s = NULL;
			AVL_Node* p = head->RChild;

			AVL_Node *q = NULL, *r = NULL;

			stack<AVL_Node*> parStk;		//Stack of parents of the element to be deleted
			parStk.push(head);

			//Locating element to be deleted
			while(p)
			{
				key_comparison++;
				if(k < p->key)			//Traverse into left subtree
				{
					parStk.push(p);
					p = p->LChild;
				}

				else if(k > p->key)		//Traverse into right subtree
				{
					parStk.push(p);
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

			s = parStk.top();			//s = parent of element to be deleted

			//Deletion of the element

			if(!p->LChild && !p->RChild)	//If element to be deleted is a leaf	
			{
				Del0Child(p, s);
			}

			else if(!p->LChild || !p->RChild)	//If element to be deleted has only 1 child
			{
				Del1Child(p, s);
				if(s->bf == 0)			//If parent is balanced post deletion, not disturbing it
					parStk.pop();
			}

			else	//If element to be deleted has 2 children
			{
				parStk.push(p);					//Pushing current element
				AVL_Node *inSucc = p->RChild;

				while(inSucc->LChild)			//Locating inorder successor of element
				{
					parStk.push(inSucc);		//Here we will push parents of inorder successor, and not the 2-degree node
					inSucc = inSucc->LChild;
				}

				p->key = inSucc->key;			//Copying inorder successor's key to the 2-degree node

				k = inSucc->key;				//Saving it in k

				AVL_Node* inSuccPar = parStk.top();		//Inorder successor's parent

				if(!inSucc->LChild && !inSucc->RChild)	//If inorder successor is leaf
				{
					Del0Child(inSucc, inSuccPar);
				}

				else									//If inorder successor has 1 child
				{
					Del1Child(inSucc, inSuccPar);
					if(s->bf == 0)						//Same as above
						parStk.pop();
				}
			}

			int a=0;	//For assignment of bf (Like in AVL_Insert)

			while(parStk.top()!=head)	//While all parents from stack are not removed
			{
				s = parStk.top();

				parStk.pop();
				t = parStk.top();	//t = parent of s

				//Same as AVL_Insert()

				key_comparison++;
				if(k < s->key)
					a = 1;
				else
					a = -1;

				if(s->bf == 1)
					r = s->LChild;
				else
					r = s->RChild;

				if(s->bf == 0)
				{
					s->bf = (-1*a);		//Element was deleted, hence imbalance introduced
					return;
				}

				else if(s->bf == a)		//key is deleted from the direction of imbalance
				{
					s->bf = 0;			//Tree became more balanced
					continue;			//Rotation still required
				}

				else if(s->bf == (-1*a))	//key is deleted from opposite direction of imbalance
				{							//That is why -a is passed
					//L-L or R-R imbalance
					if(r->bf == (-1*a))
					{
						rotation_count += singleRotation((-1*a), &p, &r, &s);
					}

					//Special case, unlike AVL_Insert()
					//Still has a single rotation
					else if(r->bf == 0)
					{
						rotation_count += 1;

						p = r;

						if(a == -1)
						{
							s->LChild = r->RChild;
							r->RChild = s;
						}
						else if(a == 1)
						{
							s->RChild = r->LChild;
							r->LChild = s;
						}

						r->bf = a;
					}

					//R-L or L-R imbalance
					else if(r->bf == a)
					{
						rotation_count += doubleRotation((-1*a), &p, &r, &s);
					}
					
					//If resulting tree has imbalance, parent won't be affected. Hence return.
					if(p->bf == 1 || p->bf == -1)
					{
						if(s == t->RChild)
							t->RChild = p;
						else
							t->LChild = p;
						return;
					}
				}

				//Same as AVL_Insert()
				if (s == t->RChild)
					t->RChild = p;
				else
					t->LChild = p;
			}
		}

		int height(AVL_Node *root)
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

		void avg_height(AVL_Node *root, float* ah, int *tn)
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

		//Destructor to delete entire tree
		~AVL_Tree()
		{
			delete head;	//Delete head, thus the entire tree under it is deleted.
		}
};

void AVL_main()
{
	fstream filein, fileout;

	fileout.open("Analysis/avl_o.csv",ios::out);

	fileout<<"AVL No. of operations,AVL Height,AVL Avg height,AVL Rotation count,AVL Key comparison count\n";

	for(int i=500;i<=10000;i+=500)
	{
		AVL_Tree *t = new AVL_Tree();

		string directory = "Test_Files/"+string(to_string(i))+".txt";
		filein.open((const char*)directory.c_str(),ios::in);

		string ch;
		int val;

		while(filein >> ch >> val)
		{
			if(ch == "Insert")
				t->AVL_Insert(val);
			
			else if(ch == "Delete")
				t->AVL_Delete(val);
		}

		filein.close();

		fileout<<i<<","<<(t->fn_height())<<","<<to_string(t->fn_avg_height())<<","<<(t->fn_rotation_count())<<","<<(t->fn_key_comparison())<<"\n";

		delete t;
	}

	fileout.close();
}