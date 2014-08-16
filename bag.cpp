//Huy Nguyen and Dorian Leu
#include <cstdlib>
#include "QueueAr.h"
#include "StackAr.h"
#include <fstream>
#include <iostream>

using namespace std;

typedef Queue <short> QPtr;

void loadc(istream *, StackAr<QPtr> *, short);
void loadb(istream *, Queue<short> *);
void unload(StackAr<QPtr>);

int main(int argc, char** argv)
{
  short max_bags;
  short bag;
 // char filename[20];
 // cin >> filename;
 // cin >> max_bags;
  StackAr<QPtr> storage;
  Queue<short> container(atoi(argv[2]));
  ifstream fin(argv[1]);
  if(fin.is_open())
  {
    while(fin >> bag && !storage.isFull())
    {
      container.makeEmpty();
      container.enqueue(bag);
      while(!container.isFull())
        if(fin >> bag)
        {
 	  container.enqueue(bag);
        }
        else break;
      storage.push(container);
    }

    while (!storage.isEmpty())
    {
      container.makeEmpty();
      container = storage.topAndPop();
      while(!container.isEmpty())
      {
        cout << container.dequeue() << " ";
      }
    }
    cout << "\n";
  }
  else 
   cout << "Failed to open file.\n";
  return 0;
}

void loadc(istream *fin, StackAr<QPtr> * storage, short max_bags)
{
  short bag; 
  while (*fin >> bag && !storage->isFull())
  {
    QPtr container(max_bags);
    container.enqueue(bag);
    loadb(fin, &container);
    storage->push(container);
  }
}

void loadb(istream* fin, Queue<short> * container)
{
  short bag;
  while (*fin>> bag && !container->isFull())
  {
    container-> enqueue(bag);
  }
}

void unload(StackAr<QPtr> storage)
{
  Queue<short> container;
  while (!storage.isEmpty())
  {
    container = storage.topAndPop();
    while(!container.isEmpty())
    {
      cout << container.dequeue();
    }
  }
}
