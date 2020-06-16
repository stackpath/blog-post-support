/* SPDX-License-Identifier: MIT */
/*
 * Copyright (c) 2020-present StackPath, LLC
 * All rights reserved.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <linux/bpf.h>
#include <bpf/libbpf.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXIT_OK   0
#define EXIT_FAIL 1

#define UNLOAD_PROG -1

#define NO_ARGS 1
#define ONE_ARG 2

char *program_basename;

static void usage(void) {
    printf("usage   (load): %s <bpf_prog.o>\n"
           "      (unload): %s\n",
           program_basename,
           program_basename);
}

int main(int argc, char **argv) {
    program_basename = basename(argv[0]);

    int err, prog_fd, tun_fd;

    struct bpf_object *obj;
    memset(&obj, 0, sizeof(obj));

    if (argc == NO_ARGS) {
        prog_fd = UNLOAD_PROG;
    } else if (argc > ONE_ARG) {
        usage();
        return EXIT_FAIL;
    } else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        usage();
        return EXIT_OK;
    } else {
        const char *filename = argv[1];

        err = bpf_prog_load(filename, BPF_PROG_TYPE_SOCKET_FILTER, &obj, &prog_fd);
        if (err) {
            fprintf(stderr, "bpf_prog_load() failed\n");
            return EXIT_FAIL;
        }
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TAP | IFF_NO_PI | IFF_MULTI_QUEUE;
    strncpy(ifr.ifr_name, "tap0", IFNAMSIZ);

    tun_fd = open("/dev/net/tun", O_RDWR);
    if (tun_fd < 0 ) {
        fprintf(stderr, "open(\"/dev/net/tun\"...) failed\n");
        return EXIT_FAIL;
    }

    err = ioctl(tun_fd, TUNSETIFF, (void *)&ifr);
    if (err) {
        fprintf(stderr, "ioctl(...TUNSETIFF...) failed: %s\n", strerror(errno));
        goto close_and_fail;
    }

    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_DETACH_QUEUE;

    err = ioctl(tun_fd, TUNSETQUEUE, (void *)&ifr);
    if (err) {
        fprintf(stderr, "ioctl(...TUNSETQUEUE...) failed: %s\n", strerror(errno));
        goto close_and_fail;
    }

    err = ioctl(tun_fd, TUNSETSTEERINGEBPF, (void *)&prog_fd);
    if (err) {
        fprintf(stderr, "ioctl(...TUNSETSTEERINGEBPF...) failed: %s\n", strerror(errno));
        goto close_and_fail;
    }

    return EXIT_OK;

close_and_fail:
    close(tun_fd);
    return EXIT_FAIL;
}
