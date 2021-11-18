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
  //Dyanmic array to hold info
  FnInfo *FnsInfoCollection;
};

/** Return pointer to opaque data structure containing collection of
 *  FnInfo's for functions which are callable directly or indirectly
 *  from the function whose address is rootFn.
 */
const FnsData *
new_fns_data(void *rootFn)
{
  //Make a new Collection and allocate memory
  struct FnsDataImpl collection;
  collection.FnsInfoCollection = malloc(sizeof(FnInfo)*(INIT_SIZE));




  //From Suggested step one in instructions

  //printf("%s\n", root);
  /*
  Lde *decoder = new_lde();
  const unsigned char *p = rootFn;
  int lineLength = get_op_length(decoder, p);
  int totalOffset = 0;
  
  while (lineLength > 1)
  {
    printf("%d\n", lineLength);
    
    //Print first byte of every line, aka the opcode
    unsigned char currentOpCode = *((unsigned char *)(p + totalOffset));
    printf("%x\n", currentOpCode);
    totalOffset += lineLength;
    lineLength = get_op_length(decoder, p + totalOffset);
  }
  printf("%d\n", lineLength);
  free_lde(decoder);
  */
 
  return NULL;
}

/** Free all resources occupied by fnsData. fnsData must have been
 *  returned by new_fns_data().  It is not ok to use to fnsData after
 *  this call.
 */
void free_fns_data(FnsData *fnsData)
{
  //todo
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

//TODO: add auxiliary functions
