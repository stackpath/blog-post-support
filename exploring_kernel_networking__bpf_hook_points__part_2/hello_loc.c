/* SPDX-License-Identifier: (GPL-2.0 OR MIT) */
/*
 * Copyright (c) 2020-present StackPath, LLC
 * All rights reserved.
 */

#include <linux/bpf.h>
#include <linux/pkt_cls.h>
#include <stdbool.h>
#include "bpf_helpers.h"

#ifdef HELLO_IP_XDPDRV
  #define LOC "ip xdpdrv"
  #define RET XDP_PASS

#elif defined(HELLO_TUNSETSTEERINGEBPF)
  #define LOC "tun.ko tunsetsteeringebpf"
  #define RET 0

#elif defined(HELLO_TC_CLSACT_INGRESS)
  #define LOC "tc clsact ingress"
  #define RET TC_ACT_OK

#elif defined(HELLO_TC_CLSACT_EGRESS)
  #define LOC "tc clsact egress"
  #define RET TC_ACT_OK

#elif defined(HELLO_IPTABLES_XT_BPF_INPUT)
  #define LOC "iptables xt_bpf input"
  #define RET false

#elif defined(HELLO_IPTABLES_XT_BPF_OUTPUT)
  #define LOC "iptables xt_bpf output"
  #define RET false

#else
  #define LOC "unspecified"
  #define RET 0

#endif

SEC("hello")
int _hello(void *ctx) {
	char fmt[] = LOC ": Hello, World!\n";
	bpf_trace_printk(fmt, sizeof(fmt));

	return RET;
}

char _license[] SEC("license") = "Dual MIT/GPL";
