CC = g++

CCFLAGS = -ansi -Wall -O3 -DNDEBUG -I../../lib
#CCFLAGS = -ansi -Wall -O6 -g -I../../lib
#CCFLAGS = -ansi -Wall -O6 -g -DALLSTATS
LDFLAGS = 
LOADLIBES = -lm        # the name used by the automatic linker rule

SRCS = main.cc sp.cc dijkstra.cc parser_gr.cc timer.cc
HDRS = sp.h nodearc.h dijkstra.h queue.h values.h
CODES = dijkstra.exe dijkstraC.exe

all: $(CODES)

dijkstra.exe: $(SRCS) $(HDRS) parser_ss.cc
	$(CC) $(CCFLAGS) -o dijkstra.exe $(SRCS) parser_ss.cc $(LOADLIBES)

dijkstraC.exe: $(SRCS) $(HDRS) parser_ss.cc
	$(CC) $(CCFLAGS) -DCHECKSUM -o dijkstraC.exe $(SRCS) parser_ss.cc $(LOADLIBES)


clean:
	rm -f *~ dijkstra.exe dijkstraC.exe
