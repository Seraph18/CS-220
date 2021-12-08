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
  free(cache);
}

/** Return result for requesting addr from cache */
CacheResult
cache_sim_result(CacheSim *cache, MemAddr addr)
{
  // Search For Address in cache; if found it is a 'CACHE_HIT'

  unsigned long tagBits = cache->numberOfBitsToSpecifyPrimaryAddress - (cache->numberOfBitsForIndex + cache->numberOfAddressBitsForOffset);

  unsigned long tag = addr >> (cache->numberOfBitsForIndex + cache->numberOfAddressBitsForOffset);

  unsigned long setIndex = (addr >> cache->numberOfAddressBitsForOffset) % cache->numberOfSets; // Set index

  printf("%lx\n", setIndex);

  for (int line = 0; line < cache->numberOfLinesPerSet; ++line)
  {
    if (cache->cacheArray[setIndex][line] == tag)
    {
      // Found a hit
      CacheResult result = {CACHE_HIT, 0};
      return result;
    }
    else if (cache->cacheArray[setIndex][line] == 0)
    {
      // it is completly empty
      cache->cacheArray[setIndex][line] = tag;
      CacheResult result = {CACHE_MISS_WITHOUT_REPLACE, 0};
      return result;
    }
  }

  switch (cache->replace)
  {
  case LRU_R:
    // todo
    break;
  case MRU_R:
    // todo
    break;
  case RANDOM_R:
    // todo
    break;
  default:
    // todo
    break;
  }

  /*for (int setIndex = 0; setIndex < cache->numberOfSets; ++setIndex)
  {
    for (int lineIndex = 0; lineIndex < cache->numberOfLinesPerSet; ++lineIndex)
    {
      if (cache->cacheArray[setIndex][lineIndex] == addr)
      {
        CacheResult result = {CACHE_HIT, 0};
        return result;
      }
      else if (cache->cacheArray[setIndex][lineIndex] == 0)
      {
        // Found a blank spot to put new value 'CACHE_MISS_WITHOUT_REPLACE'
        cache->cacheArray[setIndex][lineIndex] = addr;
        CacheResult result = {CACHE_MISS_WITHOUT_REPLACE, 0};
        return result;
      }
    }
  }
  */
  CacheResult result = {5, 0};
  return result;
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
