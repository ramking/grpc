/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef GRPC_CORE_LIB_DEBUG_TRACE_H
#define GRPC_CORE_LIB_DEBUG_TRACE_H

#include <grpc/support/atm.h>
#include <grpc/support/port_platform.h>
#include <stdbool.h>

#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
#define GRPC_THREADSAFE_TRACER
#endif
#endif

typedef struct {
#ifdef GRPC_THREADSAFE_TRACER
  gpr_atm value;
#else
  bool value;
#endif
} grpc_tracer_flag;

#ifdef GRPC_THREADSAFE_TRACER
#define GRPC_TRACER_ON(flag) (gpr_atm_no_barrier_load(&(flag).value) != 0)
#define GRPC_TRACER_INITIALIZER(on) \
  { (gpr_atm)(on) }
#else
#define GRPC_TRACER_ON(flag) ((flag).value)
#define GRPC_TRACER_INITIALIZER(on) \
  { (on) }
#endif

void grpc_register_tracer(const char *name, grpc_tracer_flag *flag);
void grpc_tracer_init(const char *env_var_name);
void grpc_tracer_shutdown(void);

#endif /* GRPC_CORE_LIB_DEBUG_TRACE_H */
