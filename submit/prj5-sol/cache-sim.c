#include "cache-sim.h"

#include "memalloc.h"
#include <stdio.h>

#include <stdbool.h>
#include <stddef.h>

struct CacheSimImpl
{
  int numberOfBitsForIndex; // s
  int numberOfSets;         // 2^s
  int numberOfLinesPerSet;  // E

  int numberOfAddressBitsForOffset; // b
  int sizeOfCache;                  // 2^b

  int numberOfBitsToSpecifyPrimaryAddress; // m
  int totalSizeOfMainMemory;               // 2^m

  Replacement replace;

  MemAddr **cacheArray; // Array to hold simulated cache
};

/** Create and return a new cache-simulation structure for a
 *  cache for main memory withe the specified cache parameters params.
 *  No guarantee that *params is valid after this call.
 */
CacheSim *
new_cache_sim(const CacheParams *params)
{
  // Give params to the cache sim
  CacheSim *cSim = malloc(sizeof(CacheSim));
  cSim->numberOfSets = power2(params->nSetBits);
  cSim->numberOfLinesPerSet = params->nLinesPerSet;

  cSim->numberOfAddressBitsForOffset = params->nLineBits;
  cSim->sizeOfCache = power2(params->nLineBits);

  cSim->numberOfBitsToSpecifyPrimaryAddress = params->nMemAddrBits;
  cSim->totalSizeOfMainMemory = power2(params->nMemAddrBits);

  cSim->replace = params->replacement;

  // Set up simulator array
  cSim->cacheArray = malloc(cSim->numberOfSets * sizeof(MemAddr));
  for (int i = 0; i < cSim->numberOfSets; ++i)
  {
    cSim->cacheArray[i] = malloc(cSim->numberOfLinesPerSet * sizeof(MemAddr));
  }

  return cSim;
}

/** Free all resources used by cache-simulation structure *cache */
void free_cache_sim(CacheSim *cache)
{
  for (int i = 0; i < cache->numberOfSets; ++i){
    free(cache->cacheArray[i]);
  }
    free(cache->cacheArray);
}

unsigned long getLineTag(CacheSim *cache, MemAddr addr)
{
  return (addr >> (cache->numberOfBitsForIndex + cache->numberOfAddressBitsForOffset));
}

/** Return result for requesting addr from cache */
CacheResult
cache_sim_result(CacheSim *cache, MemAddr addr)
{
  // Search For Address in cache; if found it is a 'CACHE_HIT'

  unsigned long tag = getLineTag(cache, addr);

  unsigned long setIndex = (addr >> cache->numberOfAddressBitsForOffset) % cache->numberOfSets; // Set index

  // printf("%lx\n", setIndex);
  //   Look for hit
  for (int line = 0; line < cache->numberOfLinesPerSet; ++line)
  {
    if (getLineTag(cache, cache->cacheArray[setIndex][line]) == tag)
    {
      // Found a hit

      // Check
      if (line != 0)
      {
        MemAddr dataToMove = cache->cacheArray[setIndex][line];

        // Move down currentValues
        for (int i = line; 0 < i; --i)
        {
          cache->cacheArray[setIndex][i] = cache->cacheArray[setIndex][i - 1];
        }

        // Readd addr
        cache->cacheArray[setIndex][0] = dataToMove;
      }

      CacheResult result = {CACHE_HIT, 0};
      return result;
    }
    else if (cache->cacheArray[setIndex][line] == 0)
    {
      // it is completly empty

      // Move down currentValues
      for (int i = line; 0 < i; --i)
      {
        cache->cacheArray[setIndex][i] = cache->cacheArray[setIndex][i - 1];
      }

      cache->cacheArray[setIndex][0] = addr;
      CacheResult result = {CACHE_MISS_WITHOUT_REPLACE, 0};
      return result;
    }
  }
  MemAddr dataToReplace;
  int randomLineIndex;
  CacheResult result = {2, 0};
  switch (cache->replace)
  {
  case LRU_R:

    // Grab data to replace for result
    dataToReplace = cache->cacheArray[setIndex][cache->numberOfLinesPerSet - 1];

    // Move down currentValues
    for (int i = cache->numberOfLinesPerSet - 1; 0 < i; --i)
    {
      cache->cacheArray[setIndex][i] = cache->cacheArray[setIndex][i - 1];
    }

    cache->cacheArray[setIndex][0] = addr;

    result.replaceAddr = dataToReplace;
    return result;

  case MRU_R:

    dataToReplace = cache->cacheArray[setIndex][0];
    cache->cacheArray[setIndex][0] = addr;
    result.replaceAddr = dataToReplace;

    return result;

  case RANDOM_R:
    
    randomLineIndex = rand() % (cache->numberOfLinesPerSet + 1);
    dataToReplace = cache->cacheArray[setIndex][randomLineIndex];
    cache->cacheArray[setIndex][randomLineIndex] = addr;

    result.replaceAddr = dataToReplace;
    return result;
  default:
    //Do Nothing if error
    break;
  }

  CacheResult defaultResult = {3, 0}; //Error Occured if this is called
  return defaultResult;
}

// Multiply 2 by the power of a given exponent
int power2(int exponent)
{
  int total = 2;

  for (int i = 0; i < exponent - 1; ++i)
  {
    total *= 2;
  }
  return total;
}
