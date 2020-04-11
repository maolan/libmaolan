#!/bin/sh

env CPPFLAGS=-I/usr/local/include CFLAGS=-I/usr/local/include CXXFLAGS=-I/usr/local/include LDFLAGS=-L/usr/local/lib ./configure
