//Dorian Leu and Huy Nguyen
#include <iostream>
#include "BTree.h"
#include "BTreeNode.h"
#include "LeafNode.h"
#include "InternalNode.h"
using namespace std;

BTree::BTree(int ISize, int LSize):internalSize(ISize), leafSize(LSize)
{
    root = new LeafNode(LSize, NULL, NULL, NULL);
} // BTree::BTree()


void BTree::insert(const int value)
{
    //cout << "Root insert\n";
    BTreeNode *ptr = root->insert(value);
    if(ptr) //split
    {
        //cout << "[Root] Split\n";
        InternalNode *nroot = new InternalNode (internalSize, leafSize, NULL, NULL, NULL);
        nroot->insert(root,ptr);
        root = nroot;
    }
} // BTree::insert()


void BTree::print()
{
    BTreeNode *BTreeNodePtr;
    Queue<BTreeNode*> queue(1000);

    queue.enqueue(root);
    while(!queue.isEmpty())
    {
        BTreeNodePtr = queue.dequeue();
        BTreeNodePtr->print(queue);
    } // while
} // BTree::print()
