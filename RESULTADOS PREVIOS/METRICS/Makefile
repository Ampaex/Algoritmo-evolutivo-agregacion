# Makefile for compiling NSGA-II source code
CC=gcc
LD=gcc
RM=rm -f
CFLAGS= -g  -D VARIANT=4
VARIANT ?= 4
OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))
MAIN=metrics
all:$(MAIN)
$(MAIN):$(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(MAIN) -lm
%.o: %.c global.h rand.h
	$(CC) $(CFLAGS) -c $<
clean:
	$(RM) $(OBJS)

