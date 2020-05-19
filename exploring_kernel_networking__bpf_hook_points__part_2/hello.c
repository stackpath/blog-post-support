/* SPDX-License-Identifier: (GPL-2.0 OR MIT) */
/*
 * Copyright (c) 2020-present StackPath, LLC
 * All rights reserved.
 */

#include <linux/bpf.h>
#include "bpf_helpers.h"

#ifdef HELLO_XDP
  #define RET XDP_PASS

#else
  #define RET 0

#endif

SEC("hello")
int _hello(void *ctx) {
	char fmt[] = "Hello, World!\n";
	bpf_trace_printk(fmt, sizeof(fmt));

	return RET;
}

char _license[] SEC("license") = "Dual MIT/GPL";
