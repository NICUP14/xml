#General makefile for C/C++ projects

#C Compiler flags
CFLAGS=-g -Wall -Wextra
#C++ Compiler flags
CXXFLAGS=$(CFLAGS)
#Project name
PROJECT=main
#Project objects
OBJECTS=main.o

#Compiling rules

#Linking rules
all: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(PROJECT)

clean:
	$(RM) $(OBJECTS) $(PROJECT)

info:
	@echo Compiler flags: $(CXXFLAGS)
	@echo Project: $(PROJECT)
	@echo Objects: $(OBJECTS)
