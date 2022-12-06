# Usage:
# make        # compile all binary

.PHONY = all

CC = g++

SRCS := main.cpp vector2.cpp
OUT := vsnake

all: build

build:
	@echo "Building snake..."
	${CC} -o ${OUT} ${SRCS} 