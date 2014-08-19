// Authors: Huy and Dorian
#include "cpu.h"
#include "HashLine.h"
#include "HashLine.cpp"
#include "mynew.h"
#include <iostream>
 
using namespace std;

CPU::CPU()
{
  ADD1 = ADD2 = ADD3 = storedbuffer = count = 0; 
  state = 1;
  soln = -1; 
  v = 0;
} // CPU()


void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer)
{
  int find;
  line Qtemp;
  switch (opCode)
  {
    case ADD  : // *addr3 = *addr1 + *addr2
      if(state == 1)
      {
        ADD1 = instruction.addr1;
        ADD2 = instruction.addr2;
        ADD3 = instruction.addr3;
        storedbuffer = buffer;
        opCode = LOAD;
      }
      break;
      
    case LOAD :  // buffer contains int requested from RAM
     // if(instruction.addr1 == 186261484)
      if(state == 1)
      {
        find = cache.addhash(instruction.addr1, buffer);
        if(find < 0)
        {
          opCode = STORE;
          instruction.addr1 = find * -1;
          buffer = cache.read(find * -1);
          break;
        }
        if(cache.getline(cache.find(instruction.addr1)).dbit)
          ADD1_dirty = true;
        state = 2;
        instruction.addr1 = ADD2;
        break;
      }
      if(state == 2)
      {
        find = cache.addhash(instruction.addr1, buffer);
        if(find < 0)
        {
          opCode = STORE;
          if((unsigned int)(find *-1) == ADD1)
          {
            instruction.addr1 = cache.getline((cache.find(ADD1)+1)%CACHE_SIZE).tag;
            buffer = cache.getline((cache.find(ADD1)+1)%CACHE_SIZE).data;
          }
          else
          {
            instruction.addr1 = find * -1;
            buffer = cache.read(find * -1);
          }
          break;
        }
        if(cache.getline(cache.find(instruction.addr1)).dbit)
          ADD2_dirty = true;
        state = 3;
        instruction.addr1 = ADD3;
        break;
      }
      if(state == 3)
      {
        if(soln == -1)
          soln = cache.read(ADD1) + cache.read(ADD2);
        if(!ADD1_dirty)
          cache.flip(ADD1);
        if(!ADD2_dirty)
          cache.flip(ADD2);
        find = cache.write(instruction.addr1, soln);
        if(find < 0)
        {
          opCode = STORE;
          instruction.addr1 = find * -1;
          buffer = cache.read(find * -1);
          break;
        }
        ADD1_dirty =ADD2_dirty = false;
        soln = -1;
        state = 1;
        opCode = ADD;
        break;
      }
      
        
    case STORE:  // Sent by RAM after a STORE
      if(state == 1)
      {
        cache.delHash(instruction.addr1);
        instruction.addr1 = ADD1;
      } 
      if(state == 2)
      {
        cache.delHash(instruction.addr1);
        instruction.addr1 = ADD2;
      } 
      if(state == 3)
      {
       cache.delHash(instruction.addr1);
       instruction.addr1 = ADD3;
      }
      if(state == 4)
      {
        if(count >= CACHE_SIZE)
	{
          opCode = DONE;
	}
        else 
        {
          Qtemp = cache.getline(count);
          while((Qtemp.tag == -1 || !Qtemp.dbit || Qtemp.lazy) && count < CACHE_SIZE)
	  {
            count++;
            Qtemp = cache.getline(count);
          }
          if(Qtemp.tag != -1 && Qtemp.dbit && !Qtemp.lazy)
	  {
	    instruction.addr1 = Qtemp.tag;
           buffer = Qtemp.data;
          }
	  else
	  {
	    opCode = DONE;
	  }
	   count++;
        }
      }
      if(state != 4)
        opCode = LOAD;
      break;

    case DONE: //  All ADDs in file have been sent.  Time to STORE dirty cache.
      state = 4;
      opCode = STORE;
      Qtemp = cache.getline(count);
      while((Qtemp.tag == -1 || !Qtemp.dbit || Qtemp.lazy) && count < CACHE_SIZE)
      {
	count++;
	Qtemp = cache.getline(count);
      }
      if(Qtemp.tag != -1 && Qtemp.dbit && !Qtemp.lazy)
      {
	instruction.addr1 = Qtemp.tag;
	buffer = Qtemp.data;
      }
      else
      {
	opCode = DONE;
      }
      instruction.addr1 = Qtemp.tag;
      buffer = Qtemp.data;
      count++;
      break;  
  } // switch
} // operation()




