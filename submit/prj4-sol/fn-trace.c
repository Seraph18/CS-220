#include "fn-trace.h"
#include "x86-64_lde.h"

#include "memalloc.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum
{
  INIT_SIZE = 2
};

struct FnsDataImpl
{
  FnInfo *functionArray;
  int currentIndex;
  int size;
};

/** Free all resources occupied by fnsData. fnsData must have been
 *  returned by new_fns_data().  It is not ok to use to fnsData after
 *  this call.
 */
void free_fns_data(FnsData *fnsData)
{
  //TODO
}

/** Iterate over all FnInfo's in fnsData.  Make initial call with NULL
 *  lastFnInfo.  Keep calling with return value as lastFnInfo, until
 *  next_fn_info() returns NULL.  Values must be returned sorted in
 *  increasing order by fnInfoP->address.
 *
 *  Sample iteration:
 *
 *  for (FnInfo *fnInfoP = next_fn_info(fnsData, NULL); fnInfoP != NULL;
 *       fnInfoP = next_fn_info(fnsData, fnInfoP)) {
 *    //body of iteration
 *  }
 *
 */
const FnInfo *
next_fn_info(const FnsData *fnsData, const FnInfo *lastFnInfo)
{
  //TODO
  return NULL;
}

/** recognized opcodes for calls and returns */
enum
{
  CALL_OP = 0xE8, //used to identify an external call which is traced

  //used to recognize the end of a function
  RET_FAR_OP = 0xCB,
  RET_FAR_WITH_POP_OP = 0xCA,
  RET_NEAR_OP = 0xC3,
  RET_NEAR_WITH_POP_OP = 0xC2
};

static inline bool is_call(unsigned op) { return op == CALL_OP; }
static inline bool is_ret(unsigned op)
{
  return op == RET_NEAR_OP || op == RET_NEAR_WITH_POP_OP ||
         op == RET_FAR_OP || op == RET_FAR_WITH_POP_OP;
}

/** Return pointer to opaque data structure containing collection of
 *  FnInfo's for functions which are callable directly or indirectly
 *  from the function whose address is rootFn.
 */
const FnsData *
new_fns_data(void *rootFn)
{

  //Initialize the FnsData
  struct FnsDataImpl collectionOfFunc;
  collectionOfFunc.functionArray = malloc(sizeof(FnInfo) * INIT_SIZE);
  collectionOfFunc.size = INIT_SIZE;
  collectionOfFunc.currentIndex = 0;

  //Initialize decoder
  Lde *decoder = new_lde();
  const unsigned char *p = rootFn; //Root address aka 'starting point'

  //Add root function to the FnsData structure
  FnInfo firstFn = {rootFn, get_op_length(decoder, rootFn), 1, 0};
  collectionOfFunc.functionArray[0] = firstFn;
  ++collectionOfFunc.currentIndex;

  fn_trace(rootFn, collectionOfFunc);
  return NULL;
}

//TODO: add auxiliary functions

void fn_trace(void *addr, FnsData collectionOfFunc)
{
  FnInfo currentFnToTrace = collectionOfFunc.functionArray[checkForExistingFn(collectionOfFunc, addr)];
  Lde *decoder = new_lde();
  const unsigned char *p = addr;
  int totalOffset = 0; //Offset of address

  unsigned char currentOpCode = *((unsigned char *)(p)); //Current opcode

  while (!is_ret(currentOpCode)) //Run until a return operation is found
  {

    currentOpCode = *((unsigned char *)(p + totalOffset)); //Set the current opcode to the correct one at the given address
    printf("%x\n", currentOpCode);                         //Print current Opcode hex value

    int lineLength = get_op_length(decoder, p + totalOffset); //Length of whole line in bits

    //check if opcode is a CALL operation
    if (is_call(currentOpCode))
    {
      ++currentFnToTrace.nOutCalls;
      //find location of the called function

      int *offSetOperandForCalledFn = (int *)(p + totalOffset + 1);
      unsigned char *addressOfNextInstruction = ((unsigned char *)(p + totalOffset + lineLength));
      unsigned int *addressOfFunctionBeingCalled = (unsigned int *)(*offSetOperandForCalledFn + addressOfNextInstruction);
      printf("%p\n", addressOfFunctionBeingCalled); //Print address of called function

      //Check if function already exists within the FnsData data structure
      int indexOfFn = checkForExistingFn(collectionOfFunc, (void *)addressOfFunctionBeingCalled);
      if (indexOfFn > -1)
      {
        //If it does, increment that nInCalls property of it
        ++collectionOfFunc.functionArray[indexOfFn].nInCalls;
      }
      else //Create a new FnInfo struct for it
      {
        FnInfo newFn = {(void *)addressOfFunctionBeingCalled, 0, 1, 0};
        collectionOfFunc.functionArray[collectionOfFunc.currentIndex] = newFn;
        ++collectionOfFunc.currentIndex;

        //Check if size needs to be increased
        if (collectionOfFunc.currentIndex == collectionOfFunc.size - 1)
        {
          collectionOfFunc.functionArray = reallocChk(collectionOfFunc.functionArray, sizeof(FnInfo) * (collectionOfFunc.size * 2));
          collectionOfFunc.size = collectionOfFunc.size * 2;
        }
        if (is_call(currentOpCode))
        {
          fn_trace((void *)addressOfFunctionBeingCalled, collectionOfFunc);
        }
      }
    }

    totalOffset += lineLength; //Adds current length to the offset for the next address call;
  }
  print(collectionOfFunc);
}

void print(FnsData fnsData)
{
  for (int i = 0; i < fnsData.currentIndex; ++i)
  {
    FnInfo tempFnInfo = fnsData.functionArray[i];
    printf("%p: nInCalls:   %d; nOutCalls:    %d; length:   %d\n", tempFnInfo.address, tempFnInfo.nInCalls, tempFnInfo.nOutCalls, tempFnInfo.length);
  }
}

//Check if function has already been seen before
int checkForExistingFn(FnsData fnsData, void *fnAddress)
{
  for (int i = 0; i < fnsData.currentIndex; ++i)
  {
    if (fnAddress == fnsData.functionArray[i].address)
    {
      return i;
    }
  }
  return -1;
}