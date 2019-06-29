CC = g++-8

CCFLAGS = -ansi -Wall -O3 -DNDEBUG --std=c++11 -fopenmp 
#CCFLAGS = -ansi -Wall -O6 -g -I../../lib
#CCFLAGS = -ansi -Wall -O6 -g -DALLSTATS
LDFLAGS = 
LOADLIBES = -lm        # the name used by the automatic linker rule

SRCS = main.cc sp.cc deltaStepping.cc parser_gr.cc timer.cc
HDRS = sp.h nodearc.h deltaStepping.h queue.h values.h
CODES = deltaStepping.exe deltaSteppingC.exe

all: $(CODES)

deltaStepping.exe: $(SRCS) $(HDRS) parser_ss.cc
	$(CC) $(CCFLAGS) -o deltaStepping.exe $(SRCS) parser_ss.cc $(LOADLIBES)

deltaSteppingC.exe: $(SRCS) $(HDRS) parser_ss.cc
	$(CC) $(CCFLAGS) -DCHECKSUM -o deltaSteppingC.exe $(SRCS) parser_ss.cc $(LOADLIBES)


clean:
	rm -f *~ deltaStepping.exe deltaSteppingC.exe
