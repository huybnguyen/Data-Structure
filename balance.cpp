//Huy Nguyen and Dorian Leu
#include <iostream>
#include <cctype>
#include "StackAr.h"
#include <fstream>
#include <string>

using namespace std;

bool balance(string input);

class bracket{
  public:
    char symb;
    int line;
};

bool check (bracket left, char right);

int main(int argc, char* argv[])
{
  string input;
  bool quote = false;
  //char filename[80];
  short count = 1;
  //cin >> filename;
  ifstream inf(argv[1]);
  StackAr <bracket> bstack;
  if(inf.is_open())
  {
    while(!inf.eof())
    {
      getline(inf,input);
      for(int i=0; i<input.length();i++)
      {
	if (input[i] == '/' && input[i+1] == '*')
	{
	  quote = true;
          bracket in;
          in.symb = 'q';
          in.line = count;
	  bstack.push(in);
	  i++;
	}
	else if (input[i] == '*' && input[i+1] == '/')
	{
	  quote = false;
	  if(bstack.isEmpty())
          {
             cout << "Unmatched */ on line #" << count <<endl;
             return 0;
          }
          else if (!check(bstack.top(), 'Q'))
          {
             cout << "Unmatched */ on line #" << count << endl;
             return 0;
          }
          else
             bstack.pop();
	  i++;
	}
        else if(!quote && (input[i] == '(' || input[i]=='{' || input[i]=='['))
        { 
          bracket in;
          in.symb = input[i];
          in.line = count;
          bstack.push(in);
        } 
        else if (!quote && (input[i] == ')' || input[i]=='}' || input[i]==']'))
        {
          if(bstack.isEmpty())
          {
            cout << "Unmatched " << input[i] << " on line #" << count << endl;
            return 0;
          }
          else if(!check(bstack.top(),input[i]))
          {
            cout << "Unmatched " << input[i] << " on line #" << count << endl;
            return 0;
          }
          else
            bstack.pop();
        }
     }
     count++;
     }
     if(bstack.isEmpty())
       cout << "OK" <<endl;
     else if(bstack.top().symb != 'Q' && bstack.top().symb != 'q')
     {
       cout << "Unmatched " << bstack.top().symb << " on line #" << bstack.top().line << endl;
     }
     else if(bstack.top().symb == 'q')
     {
       cout << "Unmatched /* on line #" << bstack.top().line << endl;
     }
     else if(bstack.top().symb == 'Q')
       cout << "Unmatched */ on line #" << bstack.top().line << endl;
     else
       cout << "Unknown error\n";
    }
    else
     cout << "Failed to open!\n";
  return 0;
}

bool check(bracket left, char right)
{
  if(left.symb == '(' && right == ')')
    return true;
  else if(left.symb == '[' && right == ']')
    return true;
  else if(left.symb == '{' && right == '}')
    return true;
  else if(left.symb == 'q' && right == 'Q')
    return true;
  return false;
}

