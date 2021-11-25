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
  free(fnsData);
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
  FnsData *collectionOfFunc = malloc(sizeof(FnsData) * 2);
  collectionOfFunc->functionArray = malloc(sizeof(FnInfo) * INIT_SIZE);
  collectionOfFunc->size = 1;
  collectionOfFunc->currentIndex = 0;

  //Add root function to the FnsData structure
  collectionOfFunc = fn_trace(rootFn, collectionOfFunc);

  qsort(collectionOfFunc->functionArray, collectionOfFunc->currentIndex, sizeof(FnInfo), cmpfunc);

  return collectionOfFunc;
}

/*
1. The first time the function is called, the second argument lastFnInfo will be passed as NULL; the function should return the address of the "first" function-info.  

2. For the next call, the client will send in the address of the "first" function-info as lastFnInfo; i.e. it will send in the return value of the previous call.  The function should return the address of the second function-info.

3. This continues on, with successive calls returning the address of successive function-info's. 

4. When there are no more function-info's left, the function should return NULL, signalling the end of the iteration to the client.*/

const FnInfo *next_fn_info(const FnsData *fnsData, const FnInfo *lastFnInfo)
{
  if (fnsData->size == 0)
  {
    return NULL;
  }
  if (lastFnInfo == NULL)
  {
    return &(fnsData->functionArray[0]);
  }
  for (int i = 0; i < fnsData->currentIndex - 1; i++)
  {
    if (fnsData->functionArray[i].address == lastFnInfo->address)
    {
      return &fnsData->functionArray[i + 1];
    }
  }

  return NULL;
}

//TODO: add auxiliary functions

FnsData *fn_trace(void *addr, FnsData *collectionOfFunc)
{
  Lde *decoder = new_lde();
  int indexOfCurrentFunction = collectionOfFunc->currentIndex;
  //Adds new Function to the collectionOfFunc
  collectionOfFunc = addFunc(addr, collectionOfFunc);  
  
  unsigned char *currentFunction = addr;
  unsigned int funcLength = 0;
  unsigned int nOut = 0;

  while (!is_ret(*currentFunction))
  {
    int instrLength = get_op_length(decoder, currentFunction);
    unsigned char opCode = *currentFunction;
    funcLength += instrLength;
    if (is_call(opCode))
    {
      ++nOut;
      int *offsetPoint = (void *)currentFunction + 1;
      void *address = *offsetPoint + currentFunction + instrLength;
      int isPresent = checkForExistingFn(collectionOfFunc, address);
      if (isPresent < 0)
      {
        collectionOfFunc = fn_trace(address, collectionOfFunc);
      }
      else
      {
        ++collectionOfFunc->functionArray[isPresent].nInCalls;
      }
    }
    currentFunction += instrLength;
  }
  //printf("%d\n", nOut);
  
  //Adds collected nOutCalls from the entire function
  collectionOfFunc->functionArray[indexOfCurrentFunction].nOutCalls = nOut;
  collectionOfFunc->functionArray[indexOfCurrentFunction].length = funcLength + 1;
  return collectionOfFunc;
}

/*
void print(FnsData fnsData)
{
  for (int i = 0; i < fnsData.currentIndex; ++i)
  {
    FnInfo tempFnInfo = fnsData.functionArray[i];
    printf("%p: nInCalls:   %d; nOutCalls:    %d; length:   %d\n", tempFnInfo.address, tempFnInfo.nInCalls, tempFnInfo.nOutCalls, tempFnInfo.length);
  }
}
*/

//Compare function for qSort

int cmpfunc(const void *a, const void *b)
{
  FnInfo aInfo = *(FnInfo *)a;
  void *address1 = aInfo.address;
  FnInfo bInfo = *(FnInfo *)b;
  void *address2 = bInfo.address;
  int dif = address1 - address2;
  return dif;
}

//Adds new function to FnsData
FnsData *addFunc(void *address, FnsData *collectionOfFunc)
{
  FnInfo *newFInfo = malloc(sizeof(FnInfo));

  newFInfo->address = address;
  newFInfo->nInCalls = 1;
  newFInfo->nOutCalls = 0;

  if (collectionOfFunc->currentIndex == collectionOfFunc->size - 1)
  {
    collectionOfFunc->functionArray = reallocChk(collectionOfFunc->functionArray, sizeof(FnInfo) * (collectionOfFunc->size * 2));
    collectionOfFunc->size = collectionOfFunc->size * 2;
  }

  collectionOfFunc->functionArray[collectionOfFunc->currentIndex] = *newFInfo;
  ++collectionOfFunc->currentIndex;

  return collectionOfFunc;
}

//Check if function has already been seen before
int checkForExistingFn(FnsData *collectionOfFunc, void *fnAddress)
{
  for (int i = 0; i < collectionOfFunc->currentIndex; ++i)
  {
    if (fnAddress == collectionOfFunc->functionArray[i].address)
    {
      return i;
    }
  }
  return -1;
}