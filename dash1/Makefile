SOURCE = dash.cpp

OBJS = $(SOURCE:.cpp=.o)

#GNU C/C++ Compiler
GCC = g++

# GNU C/C++ Linker
LINK = g++

# Compiler flags
CFLAGS = -Wall -std=c++11
CXXFLAGS = $(CFLAGS)

# Fill in special libraries needed here
LIBS = 

.PHONY: clean

# Targets include all, clean, debug, tar

all : dash

dash: $(OBJS)
	$(LINK) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o *.d core dash

debug: CXXFLAGS = -DDEBUG -g -std=c++11
debug: dash

tar: clean
	tar zcvf dash.tgz $(SOURCE) *.h Makefile

help:
	@echo "	make dash  - same as make all"
	@echo "	make all   - builds the dash target"
	@echo "	make       - same as make all"
	@echo "	make clean - remove .o .d core dash"
	@echo "	make debug - make all with -g and -DDEBUG"
	@echo "	make tar   - make a tarball of .cpp and .h files"
	@echo "	make help  - this message"

-include $(SOURCE:.cpp=.d)

%.d: %.cpp
	@set -e; /usr/bin/rm -rf $@;$(GCC) -MM $< $(CXXFLAGS) > $@
