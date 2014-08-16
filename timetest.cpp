//Huy Nguyen and Dorian Leu

#include "CPUTimer.h"
#include <fstream>
#include "CursorList.h"
#include "LinkedList.h"
#include "StackAr.h"
#include "StackLi.h"
#include "QueueAr.h"
#include "SkipList.h"

using namespace std;

void RunList(char *filename);
void RunCursorList(char *filename);
void RunStackAr(char *filename);
void RunStackLi(char *filename);
void RunQueueAr(char *filename);
void RunSkipList(char *filename);
const int MAX = 250000;
vector<CursorNode <int> > cursorSpace(250000);

int getChoice()
{
  int choice;
  do
  {
    cout<<"\n	ADT Menu";
    cout<<"\n0. Quit";
    cout<<"\n1. LinkedList";
    cout<<"\n2. CursorList";
    cout<<"\n3. StackAr";
    cout<<"\n4. StackLi";
    cout<<"\n5. QueueAr";
    cout<<"\n6. SkipList\n";
    cout<<"Your choice >> ";
    cin >> choice;
    if(choice <0 || choice >6)
      cout<<"Invalid Choice. Please try again.\n";
  } while (choice < 0 || choice >6);
  return choice;
}

int main()
{
  char filename[80];
  int choice;
  CPUTimer ct;
  cout<<"Filename >> ";
  cin >> filename;
  do
  {
    choice = getChoice(); 
    ct.reset();
    switch (choice)
    {
      case 1: RunList(filename); break;
      case 2: RunCursorList(filename); break;
      case 3: RunStackAr(filename); break;
      case 4: RunStackLi(filename); break;
      case 5: RunQueueAr(filename); break;
      case 6: RunSkipList(filename); break;
    }
    cout << "CPU time: " << ct.cur_CPUTime() << endl;
  } while (choice > 0);
}

void RunList(char *filename)
{ 
  List <int> list;
  ListItr <int> itr = list.zeroth();
  ifstream inf(filename);
  char command, string[512];
  int value; 
  inf.getline(string, 512);
  while(inf >> command >> value)
  {
    if(command == 'i')
      list.insert(value,itr);
    else
      list.remove(value);
  }
}

void RunCursorList(char *filename)
{
  CursorList <int> list(cursorSpace);
  CursorListItr <int> itr = list.zeroth();
  ifstream inf(filename);
  char command, string[512];
  int value;
  inf.getline(string,512);
  while(inf >> command >> value)
  {  
    if(command == 'i')
      list.insert(value,itr);
    else
      list.remove(value);
  }
}

void RunStackAr(char *filename)
{
  StackAr <int> stack(MAX);
  ifstream inf(filename);
  char command, string[512];
  int value;
  inf.getline(string,512);
  while(inf >> command>> value)
  {
    if(command == 'i')
     stack.push(value);
    else
     stack.pop();
  }
}

void RunStackLi(char *filename)
{
  StackLi <int> stack;
  ifstream inf(filename);
  char command, string[512];
  int value;
  inf.getline(string,512);
  while(inf >> command >> value)
  {
    if(command == 'i')
      stack.push(value);
    else
      stack.pop();
  }
}

void RunQueueAr(char *filename)
{
  Queue <int> queue(MAX);
  ifstream inf(filename);
  char command, string[512];
  int value;
  inf.getline(string,512);
  while(inf >> command >> value)
  {
    if(command == 'i')
      queue.enqueue(value);
    else
      queue.dequeue();
  }
}

void RunSkipList(char *filename)
{
  SkipList <int> list(0, MAX);
  ifstream inf(filename);
  char command, string[512];
  int value;
  inf.getline(string,512);
  while(inf >> command>> value)
  {
    if(command == 'i')
      list.insert(value);
    else
      list.deleteNode(value);
  }
}


