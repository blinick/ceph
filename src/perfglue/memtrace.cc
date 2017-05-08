
#define __USE_MEMTRACE

#include "memtrace.h"
#include "heap_profiler.h"
#include "common/environment.h"
#include "common/LogClient.h"
#include "global/global_context.h"
#include "common/debug.h"
#include "perfglue/func_track.h"

#include <gperftools/heap-profiler.h>
#include <gperftools/malloc_extension.h>
#include <gperftools/malloc_hook.h>

#include <pthread.h>
#include <stdlib.h>
#include <execinfo.h> //slb
#include "tracing/heaphooks.h"


#define dout_context g_ceph_context

pthread_key_t allochook_inprogress; //slb
pthread_key_t func_ctx; //slb

void ceph_heap_hooks_init()
{
  //SLB Hack. Use another env variable? 
  //
  tracepoint(heaphooks, allochook_hook, 67);
  //printf("Allocator hooks, ENGAGE\n");
  generic_dout(0) << "Allocator Hooks: roll out!" << dendl;
  tracepoint(heaphooks, allochook_new, (void*)0xdeadbeef, "STARTUP", 1000, 1000);
  MallocHook::AddNewHook((MallocHook_NewHook)&ceph_new_hook);
  MallocHook::AddDeleteHook((MallocHook_DeleteHook)&ceph_delete_hook);
  //printf("Allocator hooks, make it so.\n");
}

// SLB SLB
void ceph_new_hook(const void* ptr, size_t size)
{
  size_t allocsz = MallocExtension::instance()->GetAllocatedSize(ptr); 
  if (pthread_getspecific(allochook_inprogress) != (void*)0)
     return;
  pthread_setspecific(allochook_inprogress, (void*)1);
  class FTrack *curfunc = (class FTrack*)pthread_getspecific(func_ctx);
  const char *fname = curfunc!=NULL?curfunc->getfunc():"UNKNOWN\0";
  tracepoint(heaphooks, allochook_new,ptr, fname,size, allocsz);
  printf("NEW: 0x%08x, size = %d (%d), caller = (%s)\n", (unsigned int)((unsigned long long)ptr & 0xFFFFFFFF), (int)size, (int)allocsz, fname);
  pthread_setspecific(allochook_inprogress, (void*)0);
  return;
}

void ceph_delete_hook(const void* ptr)
{ 
  if (pthread_getspecific(allochook_inprogress) != (void*)0)
     return;
  pthread_setspecific(allochook_inprogress, (void*)1);
  if (NULL != ptr)
  {
    size_t sz = MallocExtension::instance()->GetAllocatedSize(ptr); 
    //printf("DEL: 0x%08x, size = %d\n", ptr, sz);
    //generic_dout(0) << ": Hook_Del: " << ptr << "sz " << sz << dendl;
    class FTrack *curfunc = (class FTrack*)pthread_getspecific(func_ctx);
    const char *fname = curfunc!=NULL?curfunc->getfunc():"UNKNOWN\0";

    tracepoint(heaphooks, allochook_del, ptr, fname, sz);
  }
  else
  {
    //printf("DEL <null>\n");
  }
  pthread_setspecific(allochook_inprogress, (void*)0);
  return;
}

