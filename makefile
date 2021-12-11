# Makefile for Grafkom running in unix based system. 
# faizauthar12

CC = gcc
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
LIBS = -lOpenGL -lglut -lGL -lGLU -lm
endif
ifeq ($(UNAME), Darwin)
LIBS = -lglut -lglfw -lGLEW -framework OpenGL
endif

%: %.cpp
	$(CC) $@.cpp $(LIBS) -o main.o -lstdc++
