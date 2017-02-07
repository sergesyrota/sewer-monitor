#Author: Arvind Pereira

# Modify the includes, sources, headers, executable and
# other flags accordingly!
# Make sure you have installed CPPUnit
# Modify the paths if required so that we can find it
INCLUDES = -I ./ -I/usr/local/include
LIBS = -L/usr/lib/ -lm -ldl -lcppunit

CC = g++
DEBUG = -g
LFLAGS = -Wall $(DEBUG)
CFLAGS = -Wall -c -std=c++0x

SOURCES = TestRunner.cpp ../LaserSensing.cpp

HEADERS = ../LaserSensing.h \
	TestLaserSensing.h

OBJ = $(SOURCES:.cpp=.o)

EXE = RunTests

all: clean $(EXE) run clean

$(EXE):	$(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LFLAGS) $(LIBS)    

run:
	./$(EXE)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

tar:
	tar -czf $(EXE).tar.gz $(SOURCES) $(HEADERS) Makefile

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
