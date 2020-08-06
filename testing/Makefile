# Makefile for testing

TARGET =  -o ../bin/fibvt

# -mcmodel=medium >2GB of static data
CFLAGS = -ggdb -Wall -mcmodel=medium

all: fibvect_test.cxx
	g++ $(CFLAGS) $(TARGET) fibvect_test.cxx
	
go:
	../bin/fibvt
