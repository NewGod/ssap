CC = g++

CCFLAGS = -ansi -Wall -O6 -DNDEBUG -I../../lib
#CCFLAGS = -ansi -Wall -O6 -g -I../../lib
#CCFLAGS = -ansi -Wall -O6 -g -DALLSTATS
LDFLAGS = 
LOADLIBES = -lm        # the name used by the automatic linker rule
MLBFLAGS = -DMLB

SRCS = main.cc sp.cc smartq.cc parser_gr.cc timer.cc
HDRS = sp.h nodearc.h smartq.h stack.h values.h
CODES = sq.exe mbp.exe sqC.exe mbpC.exe

all: $(CODES)

sq.exe: $(SRCS) $(HDRS) parser_ss.cc
	$(CC) $(CCFLAGS) -o sq.exe $(SRCS) parser_ss.cc $(LOADLIBES)

sqC.exe: $(SRCS) $(HDRS) parser_ss.cc
	$(CC) $(CCFLAGS) -DCHECKSUM -o sqC.exe $(SRCS) parser_ss.cc $(LOADLIBES)

mbp.exe: $(SRCS) $(HDRS) parser_p2p.cc
	$(CC) $(CCFLAGS) $(MLBFLAGS) -DSINGLE_PAIR -o mbp.exe $(SRCS) parser_p2p.cc $(LOADLIBES)

mbpC.exe: $(SRCS) $(HDRS) parser_p2p.cc
	$(CC) $(CCFLAGS) $(MLBFLAGS) -DCHECKSUM -DSINGLE_PAIR -o mbpC.exe $(SRCS) parser_p2p.cc $(LOADLIBES)

clean:
	rm -f *~ sq.exe mbp.exe sqC.exe mbpC.exe
