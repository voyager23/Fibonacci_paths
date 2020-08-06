# Makefile for testing

TARGET =  -o ../bin/pathfib

# -mcmodel=medium >2GB of static data
CFLAGS = -ggdb -Wall -mcmodel=medium

all: fib_paths.cxx
	g++ $(CFLAGS) $(TARGET) fib_paths.cxx
	
go:
	../bin/pathfib
