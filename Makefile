# Original makefile by Frank Barknecht.
# Taken from http://forum.pdpatchrepo.info/topic/3789/compiler-flags-for-building-an-external-in-darwin/6
# and modified to remove `ppc`.
# This file is not covered by the LICENSE file in this git repo.

NAME=squarescale
CFLAGS = -std=c99 -DPD -O3 -Wall -W -Wshadow -Wstrict-prototypes -Wno-unused -Wno-parentheses -Wno-switch -DMUUG_INTERPOLATE=1 -DMUUG_TILDE_TABLE_SIZE=512

UNIVERSAL=-arch x86_64 -arch i386
DARWINCFLAGS = $(CFLAGS) -DDARWIN $(UNIVERSAL) -pedantic
DARWIN_LIBS=$(UNIVERSAL)

linux: $(NAME).pd_linux
.SUFFIXES: .pd_linux
.c.pd_linux:
	gcc $(CFLAGS) -o $*.o -c $*.c
	ld -export_dynamics -shared -o $*.pd_linux $*.o
	strip --strip-unneeded $*.pd_linux

darwin: $(NAME).pd_darwin
.SUFFIXES: .pd_darwin
.c.pd_darwin:
	cc $(DARWINCFLAGS) -o $*.o -c $*.c
	cc -bundle -undefined suppress -flat_namespace $(DARWIN_LIBS) -o $*.pd_darwin $*.o

clean:
	rm *.o
	rm $(NAME).pd*
