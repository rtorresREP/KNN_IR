.SILENT:
CPP=g++

OBJECTS=libs/basic.o libs/bitrankw32int.o libs/kTree.o libs/Count.o\
		  libs/knnIR.o   libs/knnirNO.o  libs/knnPQ.o

BINS=   generateKtFromPointList getTime  testKNNALL getTimeAnalisis rangeAnalisis

CPPFLAGS=-std=c++11 -O3 -DNDEBUG -w -fcompare-debug-second
DEST=.

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

all: clean bin

bin: $(OBJECTS) $(BINS)

test:
	g++ $(CPPFLAGS) -o $(DEST)/test test.cpp $(OBJECTS) -lm

testC:
	g++ $(CPPFLAGS) -o $(DEST)/testC testC.cpp $(OBJECTS) -lm

testKNNALL:
	g++ $(CPPFLAGS) -o $(DEST)/testAll testKNNALL.cpp $(OBJECTS) -lm

generateKtFromPointList:
	g++ $(CPPFLAGS) -o $(DEST)/generateKtFromPointList generateKtFromPointList.cpp $(OBJECTS) -lm

getTime:
	g++ $(CPPFLAGS) -o $(DEST)/getTime getTime.cpp $(OBJECTS) -lm

getTimeAnalisis:
	g++ $(CPPFLAGS) -o $(DEST)/analisis getTimeAnalisys.cpp $(OBJECTS) -lm

rangeAnalisis:
	g++ $(CPPFLAGS) -o $(DEST)/rangeAnalisis rangeAnalisis.cpp $(OBJECTS) -lm

clean:
	rm -f $(OBJECTS) $(BINS)
	cd $(DEST); rm -f *.a $(BINS)

