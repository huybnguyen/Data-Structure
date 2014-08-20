//Dorian Leu and Huy Nguyen
#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
                   BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
    values = new int[LSize];
}  // LeafNode()

int LeafNode::findpos(int value)const
{ //cout << "Finding position\n";
    int min = 0, max  = count;

    if(value < getMinimum())
        return 0;

    while(true)
    {
        if (value == values[(min+max)/2])
            return -1;
        else if( value < values[(min + max)/2])
            max = (min + max)/2;
        else if( value > values[(min + max)/2])
            min = (min + max)/2;

        if ((max - min) <= 1)
            return max;
    }
    return -1;
} //LeafNode::findpos(int value)const

void LeafNode::rollright(int pos)
{ //cout << "[leaf] Rolling right from " << pos << endl;
    for(int i = count ; i>=pos; i--)
    {
        values[i] = values[i-1];
    }
}//LeafNode::rollright(int pos)

void LeafNode::rollleft(int pos)
{ //cout << "[leaf] Rolling left to " << pos << endl;
    for(int i=0 ; i < pos ;i++ )
    {
        values[i] = values[i + 1];
    }
} //LeafNode::rollleft(int pos)

void LeafNode::addIn(int value, int pos)
{
    rollright(pos);
    values[pos] = value;
    count++;
    if(value == values[0] && parent)
        parent->resetMinimum(this);
}

void LeafNode::addLeft(int value, int pos)
{
    if(value < values[0])
        leftSibling -> insert(value);
    else
    {
        leftSibling -> insert(values[0]);
        rollleft(pos);
        values[pos-1] = value;
        if(parent)
            parent->resetMinimum(this);
    }
}

void LeafNode::addRight(int value, int pos)
{
    if(value > values[count-1])
        rightSibling -> insert(value);
    else
    {
        rightSibling -> insert(values[count-1]);
        rollright(pos);
        values[pos] = value;
        if(value == values[0] && parent)
            parent->resetMinimum(this);
    }
}

LeafNode* LeafNode::split(int value, int pos)
{
    LeafNode *nlnode = new LeafNode (leafSize, parent, this, rightSibling);
    if(rightSibling)
        rightSibling -> setLeftSibling(nlnode);
    setRightSibling(nlnode);

    splitValues(value, nlnode, pos);

    if(parent && pos == 0)
        parent->resetMinimum(this);
    else if(pos < (leafSize+1)/2)
    {
        rollright(pos);
        values[pos]=value;
    }
    count = (leafSize+1)/2;

    return nlnode;
}

void LeafNode::splitValues(int value, LeafNode *nlnode, int pos)
{
    for(int i = (leafSize+1)/2; i < leafSize+1; i++)
    {
        if(i < pos)
        {
            nlnode -> insert(values[i]);
            values[i] = 0;
        }
        else if( i == pos)
        {
            nlnode -> insert(value);
        }
        else
        {
            nlnode -> insert (values[i-1]);
            values[i-1] = 0;
        }
    }
}

int LeafNode::getMinimum()const
{
    if(count > 0)  // should always be the case
        return values[0];
    else
        return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
    int pos;
    pos = findpos(value);    //cout << "[leaf] " << value << " at position " << pos << endl;

    if(count < leafSize)
    { //cout << "[leaf] Space available\n";
        addIn(value, pos);
        return NULL;
    }

    if(leftSibling && leftSibling -> getCount() < leafSize)
    { //cout << "[leaf] Giving to leftsibling\n";
        addLeft(value, pos);
    }
    else if(rightSibling && rightSibling -> getCount() < leafSize)
    { //cout << "[leaf] Giving to rightsibling\n";
        addRight(value, pos);
    }
    else
    { //cout << "[leaf] split"<<endl;
        return split(value, pos);
    }
    return NULL;
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
    cout << "Leaf: ";
    for (int i = 0; i < count; i++)
    {
        cout << values[i] << ' ';
    }
    cout << endl;
} // LeafNode::print()


