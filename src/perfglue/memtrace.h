
#ifndef MEMTRACE_H_
#define MEMTRACE_H_

#ifdef __USE_MEMTRACE

#include <stdlib.h>
#include "heap_profiler.h"
#include "common/environment.h"
#include "common/LogClient.h"
#include "global/global_context.h"
#include "common/debug.h"
#include "perfglue/func_track.h"

// SLB SLB
void ceph_heap_hooks_init();
void ceph_new_hook(const void* ptr, size_t size);
void ceph_delete_hook(const void* ptr);
#endif


#endif
