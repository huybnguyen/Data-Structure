//Dorian Leu and Huy Nguyen
#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
                           InternalNode *p, BTreeNode *left, BTreeNode *right) :
    BTreeNode(LSize, p, left, right), internalSize(ISize)
{
    keys = new int[internalSize]; // keys[i] is the minimum of children[i]
    children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()

void InternalNode::addIn(BTreeNode *node, int pos)
{
    children[pos] = node;
    children[pos]->setParent(this);
    keys[pos] = node->getMinimum();
    if(pos == 0 && parent)
        parent->resetMinimum(this);
} // InternalNode::addIn(BTreeNode *node, int pos)

void InternalNode::giveLeft(BTreeNode *node, int pos)
{
    if(node->getMinimum() < keys[0])
        ((InternalNode*)leftSibling) -> insert(node);
    else
    {
        ((InternalNode*)leftSibling) -> insert(children[0]);
        rollleft(pos);
        addIn(node, pos);
        parent->resetMinimum(this);
    }
} //InternalNode::giveLeft(BTreeNode *node, int pos)

void InternalNode::giveRight(BTreeNode *node, int pos)
{
    if(node->getMinimum() > keys[count-1])
        ((InternalNode*)rightSibling) -> insert(node);
    else
    {
        ((InternalNode*)rightSibling) -> insert(children[count -1]);
        rollright(pos+1);
        addIn(node, pos+1);
    }
} //InternalNode::giveRight(BTreeNode *node, int pos)

InternalNode* InternalNode::split(BTreeNode *node, int pos)
{
    InternalNode *ninode = new InternalNode (internalSize, leafSize, parent, this, rightSibling);

    if(rightSibling)
        rightSibling->setLeftSibling(ninode);
    setRightSibling(ninode);
    splitChildren(node, ninode, pos);

    if(pos+1 < (internalSize+1)/2)
    {
        rollright(pos+1);
        addIn(node, pos+1);
    }
    count = (internalSize+1)/2;

    return ninode;
} //InternalNode::split(BTreeNode *node, int pos)

void InternalNode::splitChildren(BTreeNode *node, InternalNode * ninode, int pos)
{
    for(int i = (internalSize+1)/2; i < internalSize+1; i++) //send over half of the children
    {
        if(i < pos+1)
        { //cout << "[internal] Sending child i (" << i << ") to new sibling\n";
            ninode->insert(children[i]);
            children[i] = NULL;
            keys[i] = 0;
        }
        else if(i == pos+1)
        { //cout << "[internal] Sending new node to new sibling\n";
            ninode->insert(node);
        }
        else
        { //cout << "[internal] Sending child i-1 (" << i-1 << ") to new sibling\n";
            ninode->insert (children[i-1]);
            children[i-1] = NULL;
            keys[i-1] = 0;
        }
    }
} //InternalNode::splitChildren(BtreeNode *node, internalNode * ninode, int pos)

int InternalNode::getMinimum()const
{
    if(count > 0)   // should always be the case
        return children[0]->getMinimum();
    else
        return 0;
} // InternalNode::getMinimum()

int InternalNode::findpos(int value)const
{ //cout << "[internal] Finding position\n";
    int min = 0, max  = count-1;

    if(value > keys[count-1])
        return count-1;

    while(true) //Binary search
    {
        if(value == keys[(min+max)/2])
            return -1;
        else if( value < keys[(min + max)/2])
            max = (min + max)/2;
        else if( value > keys[(min + max)/2])
            min = (min + max)/2;

        if ((max - min) <= 1)
            return min;
    } //Binary search
    return -1;
} //InternalNode::findpos(int value)const

void InternalNode::rollright(int pos)
{ //cout << "[internal] Rolling right from " << pos << endl;
    for(int i = count; i >= pos; i--)
    {
        children[i+1] = children[i];
        keys[i+1] = keys[i];
    }
} //InternalNode::rollright(int pos)

void InternalNode::rollleft(int pos)
{ //cout << "[internal] Rolling Left to " << pos << endl;
    for(int i=0; i < pos ; i++)
    {
        children[i] = children[i+1];
        keys[i] = keys[i+1];
    }
} //InternalNode::rollleft(int pos)

InternalNode* InternalNode::insert(int value)
{
    int pos;

    pos = findpos(value);    //cout << "[internal] Inserting " << value << " at " << pos << endl;
    BTreeNode *ptr = children[pos]->insert(value);

    if(!ptr)
        return NULL;

    if(count < internalSize)
    { //cout << "[Internal] Space available\n";
        rollright(pos+1);
        addIn(ptr, pos+1);
        count++;
        return NULL;
    }

    if(leftSibling &&leftSibling -> getCount() < internalSize)
    { //cout << "[internal] Giving to leftSibling\n";
        giveLeft(ptr, pos);
    }
    else if(rightSibling && rightSibling -> getCount() < internalSize)
    { //cout << "[internal] Giving to rightSibling\n";
        giveRight(ptr, pos);
    }
    else
    { //cout << "[internal] Split\n";
        return split(ptr, pos);
    }
    return NULL;
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ //cout << "[internal] Inserting due to root split\n";
    addIn(oldRoot, 0);
    addIn(node2, 1);
    children[0]->setLeftSibling(NULL);
    children[0]->setRightSibling(node2);
    children[1]->setLeftSibling(oldRoot);
    children[1]->setRightSibling(NULL);
    count = 2;
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{ //cout << "[internal] Inserting from sibling\n";
    if (count == 0)
    {
        addIn(newNode, 0);
        count = 1;
    }
    else if (newNode->getMinimum() < keys[0])
    {
        rollright(0);
        addIn(newNode, 0);
        count++;
    }
    else if(newNode->getMinimum() > keys[count-1])
    {
        addIn(newNode, count);
        count++;
    }
} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
    int i;

    cout << "Internal: ";
    for (i = 0; i < count; i++)
        cout << keys[i] << ' ';
    cout << endl;

    for(i = 0; i < count; i++)
        queue.enqueue(children[i]);
} // InternalNode::print()

void InternalNode::resetMinimum(const BTreeNode* child)
{
    for(int i=0; i<count; i++)
        if(children[i]==child)
        {
            keys[i]=children[i]->getMinimum();
            if(i==0 && parent)
                parent->resetMinimum(this);
            break;
        }
}
