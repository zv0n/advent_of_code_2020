CXX ?= c++
CXXFLAGS ?= -std=c++14 -Wall -Wextra -pedantic -O2
PROJECT = seats

all: ${PROJECT}

${PROJECT}: main.cpp
	${CXX} ${CXXFLAGS} -o $@ $^

test: ${PROJECT}
	./${PROJECT}

clean:
	${RM} *.o ${PROJECT}

.PHONY: all clean test
