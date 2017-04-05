
#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER heaphooks

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./tracing/heaphooks.h"

#if !defined(TRACING_HEAPHOOKS_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define TRACING_HEAPHOOKS_H

#include <lttng/tracepoint.h>

#include "tracing/tracing-common.h"
#include "include/int_types.h"


TRACEPOINT_EVENT(heaphooks, allochook_hook,
    TP_ARGS(
        unsigned char,  on ),
    TP_FIELDS(
        ctf_integer(unsigned char, on, on)
    )
)

TRACEPOINT_EVENT(heaphooks, allochook_new,
    TP_ARGS(
        const void *,  ptr,
        const char *,  caller,
        size_t,  size,
        size_t,  allocsize),
    TP_FIELDS(
        ctf_integer(const void *, ptr, ptr)
        ctf_string(caller, caller)
        ctf_integer(size_t, size, size)
        ctf_integer(size_t, allocsize, allocsize)
    )
)


TRACEPOINT_EVENT(heaphooks, allochook_del,
    TP_ARGS(
        const void *,  ptr,
        const char *,  caller,
        size_t,  allocsize),
    TP_FIELDS(
        ctf_integer(const void *, ptr, ptr)
        ctf_string(caller, caller)
        ctf_integer(size_t, allocsize, allocsize)
    )
)


#endif /* TRACING_HEAPHOOKS_H */

#include <lttng/tracepoint-event.h>
