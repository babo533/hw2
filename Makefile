COMPILER=g++
FLAGS=-g -Wall -std=c++11
# Uncomment for parser DEBUG
#DEFS=-DDEBUG

OBJECTS=mydatastore.o movie.o clothing.o book.o util.o product_parser.o db_parser.o user.o amazon.o product.o

all: amazon

amazon: $(OBJECTS)
	$(COMPILER) $(FLAGS) $(DEFS) -o $@ $(OBJECTS)

mydatastore.o: mydatastore.cpp mydatastore.h datastore.h util.h user.h product.h product_parser.h
	$(COMPILER) $(FLAGS) $(DEFS) -o $@ -c mydatastore.cpp

movie.o: movie.cpp movie.h product.h util.h
	$(COMPILER) $(FLAGS) $(DEFS) -o $@ -c movie.cpp

clothing.o: clothing.cpp clothing.h product.h util.h
	$(COMPILER) $(FLAGS) $(DEFS) -o $@ -c clothing.cpp

book.o: book.cpp book.h product.h util.h
	$(COMPILER) $(FLAGS) $(DEFS) -o $@ -c book.cpp

util.o: util.cpp util.h
	$(COMPILER) $(FLAGS) $(DEFS) -o $@ -c util.cpp

product_parser.o: product_parser.cpp product_parser.h product.h book.h clothing.h movie.h util.h
	$(COMPILER) $(FLAGS) $(DEFS) -o $@ -c product_parser.cpp

db_parser.o: db_parser.cpp db_parser.h product.h product_parser.h user.h datastore.h
	$(COMPILER) $(FLAGS) $(DEFS) -o $@ -c db_parser.cpp

user.o: user.cpp user.h
	$(COMPILER) $(FLAGS) $(DEFS) -o $@ -c user.cpp

amazon.o: amazon.cpp db_parser.h datastore.h product_parser.h product.h util.h book.h movie.h clothing.h mydatastore.h
	$(COMPILER) $(FLAGS) $(DEFS) -o $@ -c amazon.cpp

product.o: product.cpp product.h
	$(COMPILER) $(FLAGS) $(DEFS) -o $@ -c product.cpp

clean:
	rm -f *.o amazon
