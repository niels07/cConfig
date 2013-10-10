# 
# Makefile for cConfig
#
# Copyright (c) 2013 "cConfig" Niels Vanden Eynde 
# 
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:

# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#

CC=gcc
CFLAGS=-c -g -Wall -fpic
OBJ=lib.o hash.o cConfig.o
INSTALL_DIR=/usr/lib/
OUT=libcConfig.so

all: cConfig

cConfig: lib.o hash.o cConfig.o
	$(CC) -shared -o $(OUT) $(OBJ)

lib.o:	lib.c lib.h
	$(CC) $(CFLAGS) lib.c

hash.o:	hash.c hash.h
	$(CC) $(CFLAGS) hash.c

cConfig.o: cConfig.c cConfig.h
	$(CC) $(CFLAGS) cConfig.c

install: $(OUT)  
	cp $(OUT) $(INSTALL_DIR)
	cp cConfig.h /usr/include/ 

examples: examples/mail examples/simple 
	$(CC) -g examples/mail.c -o examples/mail -lcConfig
	$(CC) -g examples/simple.c -o examples/simple -lcConfig

clean:
	rm -f $(OBJ)
	rm -f $(OUT)
	rm -f examples/mail
	rm -f examples/simple
