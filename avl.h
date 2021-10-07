#include <iostream>
#include <limits.h>
#include <queue>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
#include <unordered_map>
#include <fstream>
#include <cmath>

using namespace std;

class AVL_Node
{
private:
    int key;
    int bf; // height of left subtree - height of right subtree
    AVL_Node *LChild, *RChild;

public:
    AVL_Node(int data)
    {
        key = data;
        LChild = nullptr;
        RChild = nullptr;
        bf = 0;
    }
    friend class AVL_Tree;
};

class AVL_Tree
{
private:
    AVL_Node *head; // head points to dummy node
    void AVL_RR_Rotation(AVL_Node *, AVL_Node *, int);
    void AVL_LL_Rotation(AVL_Node *, AVL_Node *, int);
    void AVL_LR_Rotation(AVL_Node *, AVL_Node *, int, string);
    void AVL_RL_Rotation(AVL_Node *, AVL_Node *, int, string);
    AVL_Node *copyTree(AVL_Node *, unordered_map<AVL_Node *, AVL_Node *> &);

    int rotation_count;
    int key_comparison;

public:
    AVL_Tree()
    {
        head = new AVL_Node(INT_MAX);
    }

    //copy constructor
    AVL_Tree(const AVL_Tree &tree)
    {
        if (tree.head->RChild == nullptr) //if tree is empty store null
            head->RChild = nullptr;
        unordered_map<AVL_Node *, AVL_Node *> map;       // hashmap to map the nodes of two trees
        head->RChild = copyTree(tree.head->RChild, map); //invoke the copyTree function
    }

    void AVL_Insert(int);
    void AVL_Delete(int);
    int height(AVL_Node*);
    int fn_height();
    void avg_height(AVL_Node*,float*,int*);
    float fn_avg_height();
    int fn_key_comparison();
    int fn_rotation_count();
    ~AVL_Tree()
    {
        delete head;
    }
};

AVL_Node *AVL_Tree ::copyTree(AVL_Node *root, unordered_map<AVL_Node *, AVL_Node *> &map)
{
    AVL_Node *newNode = new AVL_Node(root->key);       // new node with same data as root
    map[root] = newNode;                               // store it in map
    if (root->LChild != nullptr)                       // if root has left child
        newNode->LChild = copyTree(root->LChild, map); // invoke recursion on left child
    if (root->RChild != nullptr)                       // if root has right child
        newNode->RChild = copyTree(root->RChild, map); // invoke recursion on right child
    return newNode;
}

void AVL_Tree::AVL_Insert(int k)
{
    AVL_Node *newNode = new AVL_Node(k);
    AVL_Node *root = head->RChild;

    // first insertion
    if (root == nullptr)
    {
        head->RChild = newNode;
        return;
    }

    // rebalancingPoint points to node where rebalancing may be necessary
    // parent points to parent of rebalancingPoint
    // cursor is used for traversal
    // cursorNext point to the child of cursor
    AVL_Node *parent = head, *rebalancingPoint = root;
    AVL_Node *cursor = root, *cursorNext;

    while (true)
    {
        key_comparison++;
        if (k < cursor->key) // if k is smaller than cursor->key
        {
            cursorNext = cursor->LChild;
            if (cursorNext == nullptr) // if cursor reached the leaf node
            {
                cursorNext = newNode;
                cursor->LChild = cursorNext; // make newNode as left child of cursor
                break;
            }
            else if (cursorNext->bf != 0) // if balance factor of cursorNext is non-zero
            {
                parent = cursor;
                rebalancingPoint = cursorNext;
            }
            cursor = cursorNext;
        }
        else if (k > cursor->key) // if k is greater than cursor->key
        {
            cursorNext = cursor->RChild;
            if (cursorNext == nullptr) // if cursor reached the leaf node
            {
                cursorNext = newNode;
                cursor->RChild = cursorNext; // make newNode as right child of cursor
                break;
            }
            else if (cursorNext->bf != 0) // if balance factor of cursorNext is non-zero
            {
                parent = cursor;
                rebalancingPoint = cursorNext;
            }
            cursor = cursorNext;
        }
        else                    // if k already exists in the tree
            return;
            //throw newNode->key; // throw exception
    }

    // a denotes in which side of rebalancing point the insertion took place
    key_comparison++;
    int a = k < rebalancingPoint->key ? 1 : -1;
    cursor = (a == 1) ? rebalancingPoint->LChild : rebalancingPoint->RChild;
    /* temp points to the insertion side child of rebalancing point
        i.e., if insertion happened on left side then temp point to left child
        else right child */
    AVL_Node *temp = cursor;

    // updating the balance factor of nodes until cursor != cursorNext
    while (cursor != cursorNext)
    {
        key_comparison++;
        if (k < cursor->key)
        {
            cursor->bf = 1;
            cursor = cursor->LChild;
        }
        else if (k > cursor->key)
        {
            cursor->bf = -1;
            cursor = cursor->RChild;
        }
    }

    // if balance factor of rebalancing point is 0
    if (rebalancingPoint->bf == 0)
    {
        rebalancingPoint->bf = a; //set the new balance factor in it
        return;
    }
    /* if balance factor of rebalancing point is opposite to the new
     balance factor i.e., tree is more balanced now */
    else if (rebalancingPoint->bf == -1 * a)
    {
        rebalancingPoint->bf = 0;
        return;
    }
    else // rebalancingPoint->bf == a
    {
        /* insertion is done on the same side of temp as that of rebalancing point
           so, single rotation is required */
        if (temp->bf == a)
        {
            cursor = temp;
            /* insertion is done in the left side of rebalancing point
               and left side of temp */
            if (a == 1)
                AVL_LL_Rotation(rebalancingPoint, temp, a);
            /* insertion is done in the right side of rebalancing point
               and right side of temp */
            else if (a == -1)
                AVL_RR_Rotation(rebalancingPoint, temp, a);
        }
        /* insertion is done on the opposite side of temp as that of rebalancing point
           double rotation  */
        else if (temp->bf == -1 * a)
        {
            /* insertion is done in the left side of rebalancing point
               and right side of temp */
            if (a == 1)
            {
                cursor = temp->RChild;
                AVL_LR_Rotation(rebalancingPoint, temp, a, "insertion");
            }
            /* insertion is done in the right side of rebalancing point
               nd left side of temp */
            else if (a == -1)
            {
                cursor = temp->LChild;
                AVL_RL_Rotation(rebalancingPoint, temp, a, "insertion");
            }
        }
    }

    if (rebalancingPoint == parent->RChild)
        parent->RChild = cursor;
    else
        parent->LChild = cursor;
}

void AVL_Tree::AVL_RL_Rotation(AVL_Node *rebalancingPoint, AVL_Node *temp, int a, string operation)
{
    rotation_count+=2;
    // first rotaion
    AVL_Node *cursor = temp->LChild;
    temp->LChild = cursor->RChild;
    cursor->RChild = temp;

    // second rotation
    rebalancingPoint->RChild = cursor->LChild;
    cursor->LChild = rebalancingPoint;

    // update balance factor
    if (operation.compare("insertion") == 0)
    {
        rebalancingPoint->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? 0 : 1);
        temp->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? -1 : 0);
    }
    else if (operation.compare("deletion") == 0)
    {
        rebalancingPoint->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? 0 : 1);
        temp->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? -1 : 0);
    }
    cursor->bf = 0;
}

void AVL_Tree::AVL_LR_Rotation(AVL_Node *rebalancingPoint, AVL_Node *temp, int a, string operation)
{
    rotation_count+=2;
    // first rotation
    AVL_Node *cursor = temp->RChild;
    temp->RChild = cursor->LChild;
    cursor->LChild = temp;

    // second rotation
    rebalancingPoint->LChild = cursor->RChild;
    cursor->RChild = rebalancingPoint;

    // update balance factor
    if (operation.compare("insertion") == 0)
    {
        rebalancingPoint->bf = (cursor->bf == 0) ? 0 : (cursor->bf == 1 ? -1 : 0);
        temp->bf = (cursor->bf == 0) ? 0 : (cursor->bf == 1 ? 0 : 1);
    }
    else if (operation.compare("deletion") == 0)
    {
        rebalancingPoint->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? -1 : 0);
        temp->bf = cursor->bf == 0 ? 0 : (cursor->bf == 1 ? 0 : 1);
    }
    cursor->bf = 0;
}

void AVL_Tree::AVL_RR_Rotation(AVL_Node *rebalancingPoint, AVL_Node *temp, int a)
{
    rotation_count+=1;
    // rotation
    AVL_Node *cursor = temp;
    rebalancingPoint->RChild = temp->LChild;
    temp->LChild = rebalancingPoint;

    // update balance factor
    if (temp->bf == 0)
    {
        temp->bf = a;
    }
    else
    {
        rebalancingPoint->bf = 0;
        temp->bf = 0;
    }
}

void AVL_Tree::AVL_LL_Rotation(AVL_Node *rebalancingPoint, AVL_Node *temp, int a)
{
    rotation_count+=1;
    // rotation
    AVL_Node *cursor = temp;
    rebalancingPoint->LChild = temp->RChild;
    temp->RChild = rebalancingPoint;

    // update balance factor
    if (temp->bf == 0)
    {
        temp->bf = a;
    }
    else
    {
        rebalancingPoint->bf = 0;
        temp->bf = 0;
    }
}

void AVL_Tree::AVL_Delete(int k)
{
    stack<AVL_Node *> stack; // store the nodes which comes in the path upto the deleted node
    stack.push(head);
    AVL_Node *root = head->RChild; // root is the root of the tree which is right child of head node
    AVL_Node *curr = root;         // curr used for path traversal
    AVL_Node *prev = head;         //prev points to prev of curr

    // finding the node with key = k
    while (curr != nullptr && curr->key != k)
    {
        key_comparison++;

        prev = curr;             // updating prev
        stack.push(prev);        // pushing curr into stack
        if (curr->key < k)       // if curr->key is smaller than k
            curr = curr->RChild; // update curr to right child of curr
        else if (curr->key > k)  // if curr->key is greater than k
            curr = curr->LChild; // update curr to right child of curr
    }

    // k doesn't exists
    if (curr == nullptr)
        return;
        //throw k;

    // leaf node deletion i.e., k is present in the leaf node
    if (curr->LChild == nullptr && curr->RChild == nullptr)
    {
        if (prev == head) // if deleted node is root node itself with one child
            head->RChild = curr->RChild;
        else if (curr->key < prev->key) // if node to be deleted is left child of prev
        {
            key_comparison++;
            prev->LChild = nullptr;
        }
        else // if node to be deleted is right child of prev
        {
            key_comparison++;
            prev->RChild = nullptr;
        }
        // free(curr);
    }
    // node with single child deletion i.e., k has one child
    else if (curr->LChild == nullptr || curr->RChild == nullptr)
    {
        AVL_Node *temp;
        // if curr has no left child
        if (curr->LChild == nullptr)
            temp = curr->RChild;
        else // otherwise
            temp = curr->LChild;

        if (prev == head) // if deleted node is root node itself with one child
            head->RChild = temp;
        else if (curr->key > prev->key) // if the node to be deleted is right child of prev
        {
            key_comparison++;
            prev->RChild = temp;
        }
        else // if the node to be deleted is left child of prev
        {
            key_comparison++;
            prev->LChild = temp;
        }
        // free(curr);
    }
    // node with double child deletion i.e., k has two children
    else
    {
        AVL_Node *temp;                // temp is used for traversal to find out inorder successor of curr
        AVL_Node *parOfTemp = nullptr; // parOfTemp points to parent of temp
        temp = curr->RChild;
        stack.push(curr);

        // finding inorder successor of curr
        while (temp->LChild != nullptr)
        {
            stack.push(temp);
            parOfTemp = temp;
            temp = temp->LChild;
        }

        /* if the right child of curr is itself the
            inorder successor of curr */
        if (parOfTemp == nullptr)
            curr->RChild = temp->RChild;
        else
            parOfTemp->LChild = temp->RChild;
        curr->key = temp->key; // copying the key value
        k = temp->key;
        // free(temp);
    }

    // rebalancingPoint points to node where rebalancing may be necessary
    // parent points to parent of rebalancingPoint
    // cursor is used for traversal
    // temp point to the child of rebalancingPoint
    AVL_Node *rebalancingPoint, *parent;
    AVL_Node *temp, *cursor;

    while (stack.top() != head)
    {
        rebalancingPoint = stack.top();

        key_comparison++;
        int a = k < rebalancingPoint->key ? 1 : -1; // a denotes in which side of rebalancing point the deletion took place
        
        stack.pop();
        parent = stack.top();

        if (rebalancingPoint->bf == a) //if balance factor of rebalancingPoint is same as new balance factor
        {
            rebalancingPoint->bf = 0;
            continue;
        }
        // if balance factor of rebalancing point is 0
        else if (rebalancingPoint->bf == 0)
        {
            rebalancingPoint->bf = -1 * a;
            return;
        }
        //if (rebalancingPoint->bF == -1*a) i.e., if balance factor of rebalancingPoint is opposite of a
        else
        {
            temp = rebalancingPoint->bf == 1 ? rebalancingPoint->LChild : rebalancingPoint->RChild;
            //single rotation when temp has one child
            if (temp->bf == -1 * a)
            {
                cursor = temp;
                if (a == -1)
                    AVL_LL_Rotation(rebalancingPoint, temp, a);
                else if (a == 1)
                    AVL_RR_Rotation(rebalancingPoint, temp, a);
            }
            //single rotation when temp has two children
            else if (temp->bf == 0)
            {
                cursor = temp;
                if (a == -1)
                    AVL_LL_Rotation(rebalancingPoint, temp, a);
                else if (a == 1)
                    AVL_RR_Rotation(rebalancingPoint, temp, a);
            }
            //double rotation
            else if (temp->bf == a)
            {
                if (a == -1)
                {
                    cursor = temp->RChild;
                    AVL_LR_Rotation(rebalancingPoint, temp, a, "deletion");
                }
                else if (a == 1)
                {
                    cursor = temp->LChild;
                    AVL_RL_Rotation(rebalancingPoint, temp, a, "deletion");
                }
            }

            // No further rotations required if the balance factor has not changed for the subtree
            if (cursor->bf == 1 || cursor->bf == -1)
            {
                if (rebalancingPoint == parent->RChild)
                    parent->RChild = cursor;
                else
                    parent->LChild = cursor;
                return;
            }
        }

        if (rebalancingPoint == parent->RChild)
            parent->RChild = cursor;
        else
            parent->LChild = cursor;
    }
}

int AVL_Tree::height(AVL_Node *root){

    if(!root)
        return 0;

    int lHeight = height(root->LChild);
    int rHeight = height(root->RChild);

    if(lHeight > rHeight)
        return (lHeight+1);
    else
        return (rHeight+1);
}

int AVL_Tree::fn_height(){

    return height(head->RChild);
}

void AVL_Tree::avg_height(AVL_Node *root, float* ah, int *tn){

    if(!root) return;

    avg_height(root->LChild, ah, tn);
    (*ah) += height(root);
    (*tn)++;
    avg_height(root->RChild, ah, tn);

}

float AVL_Tree::fn_avg_height(){

    float ah = 0;
    int tn = 0;
    avg_height(head->RChild, &ah, &tn);

    return (ah/tn);
}

int AVL_Tree::fn_rotation_count(){

    return rotation_count;
}

int AVL_Tree::fn_key_comparison()
{
    return key_comparison;
}

void AVL_main()
{
    fstream filein, fileout;

    fileout.open("Analysis/avl_o.csv",ios::out);

    fileout<<"AVL No. of operations,AVL Height,AVL Avg height,AVL Rotation count,AVL Key comparison count\n";

    AVL_Tree *t;

    for(int i=500;i<=10000;i+=500)
    {
        t = new AVL_Tree();

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