# bpf progs

CLANG = clang
CLANGFLAGS = -O2 -g -S -emit-llvm

LLC = llc
LLCFLAGS = -march=bpf -filetype=obj

## hello.c

OFILES = \
  hello.o \
  hello_xdp.o

.PHONY: hello
hello: $(OFILES)

hello.o: hello.c
	$(CLANG) $(CLANGFLAGS)             -c $< -o - | llc $(LLCFLAGS) -o $@

hello_xdp.o: hello.c
	$(CLANG) $(CLANGFLAGS) -DHELLO_XDP -c $< -o - | llc $(LLCFLAGS) -o $@

## hello_loc.c

OFILES_LOC = \
  hello_ip_xdpdrv.o \
  hello_tunsetsteering.o \
  hello_tc_clsact_ingress.o \
  hello_tc_clsact_egress.o \
  hello_iptables_xt_bpf_input.o \
  hello_iptables_xt_bpf_output.o

.PHONY: hello_loc
hello_loc: $(OFILES_LOC)

hello_ip_xdpdrv.o: hello_loc.c
	$(CLANG) $(CLANGFLAGS) -DHELLO_IP_XDPDRV              -c $< -o - | llc $(LLCFLAGS) -o $@

hello_tunsetsteering.o: hello_loc.c
	$(CLANG) $(CLANGFLAGS) -DHELLO_TUNSETSTEERINGEBPF     -c $< -o - | llc $(LLCFLAGS) -o $@

hello_tc_clsact_ingress.o: hello_loc.c
	$(CLANG) $(CLANGFLAGS) -DHELLO_TC_CLSACT_INGRESS      -c $< -o - | llc $(LLCFLAGS) -o $@

hello_tc_clsact_egress.o: hello_loc.c
	$(CLANG) $(CLANGFLAGS) -DHELLO_TC_CLSACT_EGRESS       -c $< -o - | llc $(LLCFLAGS) -o $@

hello_iptables_xt_bpf_input.o: hello_loc.c
	$(CLANG) $(CLANGFLAGS) -DHELLO_IPTABLES_XT_BPF_INPUT  -c $< -o - | llc $(LLCFLAGS) -o $@

hello_iptables_xt_bpf_output.o: hello_loc.c
	$(CLANG) $(CLANGFLAGS) -DHELLO_IPTABLES_XT_BPF_OUTPUT -c $< -o - | llc $(LLCFLAGS) -o $@

# loader

CC = gcc
CFLAGS = -Wall -Wcast-align -Wcast-qual -Wimplicit -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow

## tunsetsteeringebpf_loader.c

LOADER = \
  tunsetsteeringebpf-loader

loader: $(LOADER)

tunsetsteeringebpf-loader: tunsetsteeringebpf_loader.c
	$(CC) $(CFLAGS) -o $@ $< -lbpf -lelf -lz

# clean

.PHONY: clean
clean:
	rm -f $(OFILES) $(OFILES_LOC) $(LOADER)
