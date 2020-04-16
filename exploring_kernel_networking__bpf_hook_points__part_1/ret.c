/* SPDX-License-Identifier: (GPL-2.0 OR MIT) */
/*
 * Copyright (c) 2020-present StackPath, LLC
 * All rights reserved.
 */

#include <linux/bpf.h>
#include "bpf_helpers.h"

SEC("ret")
int _ret(void *ctx) {
	return 0;
}

char _license[] SEC("license") = "Dual MIT/GPL";
