#include <heaplayers.h>

#include "tprintf.h"
#include "common.hpp"

#include "mmaparray.hpp"
#include "dynarray.hpp"
#include "stack.hpp"
#include "buffer.hpp"
#include "classwarfare.hpp"
#include "bufferbump.hpp"
#include "cheapheap.hpp"
#include "sampleheap.hpp"

#include "repoman.hpp"

class TheCustomHeap;
static TheCustomHeap * theCustomHeap = nullptr;

const auto MallocSamplingRate = 1024 * 1024;
const auto FreeSamplingRate   = 1024 * 1024;
const auto RepoSize = 4096;

typedef SampleHeap<MallocSamplingRate, FreeSamplingRate, RepoMan<RepoSize>> CustomHeapType;
//typedef SampleHeap<MallocSamplingRate, FreeSamplingRate, RepoMan<RepoSize>> CustomHeapType;

class TheCustomHeap : public CustomHeapType {
  typedef CustomHeapType Super;
public:
  TheCustomHeap() {
    theCustomHeap = this;
  }
  inline void * malloc(size_t sz) {
    auto ptr = Super::malloc(sz);
    return ptr;
  }
  inline void free(void * ptr) {
    Super::free(ptr);
  }
};

TheCustomHeap& getTheCustomHeap() {
  static TheCustomHeap thang;
  return thang;
}

extern "C" void * xxmalloc(size_t sz) {
  if (theCustomHeap) {
    return theCustomHeap->malloc(sz);
  } else {
    return getTheCustomHeap().malloc(sz);
  }
}

extern "C" void xxfree(void * ptr) {
  theCustomHeap->free(ptr);
}

extern "C" void xxfree_sized(void * ptr, size_t sz) {
  // TODO FIXME maybe make a sized-free version?
  getTheCustomHeap().free(ptr);
}

extern "C" size_t xxmalloc_usable_size(void * ptr) {
  return theCustomHeap->getSize(ptr); // TODO FIXME adjust for ptr offset?
}

extern "C" void xxmalloc_lock() {
}

extern "C" void xxmalloc_unlock() {
}
