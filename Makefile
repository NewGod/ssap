CC = g++

CCFLAGS = -ansi -Wall -O6 -DNDEBUG -I../../lib
#CCFLAGS = -ansi -Wall -O6 -g -I../../lib
#CCFLAGS = -ansi -Wall -O6 -g -DALLSTATS
LDFLAGS = 
LOADLIBES = -lm        # the name used by the automatic linker rule

SRCS = main.cc sp.cc spfa.cc parser_gr.cc timer.cc
HDRS = sp.h nodearc.h spfa.h queue.h values.h
CODES = spfa.exe spfaC.exe

all: $(CODES)

spfa.exe: $(SRCS) $(HDRS) parser_ss.cc
	$(CC) $(CCFLAGS) -o spfa.exe $(SRCS) parser_ss.cc $(LOADLIBES)

spfaC.exe: $(SRCS) $(HDRS) parser_ss.cc
	$(CC) $(CCFLAGS) -DCHECKSUM -o spfaC.exe $(SRCS) parser_ss.cc $(LOADLIBES)


clean:
	rm -f *~ spfa.exe spfaC.exe
