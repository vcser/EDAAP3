all: sa fm

sa: sa.cpp
	g++ sa.cpp -o sa -lsdsl -ldivsufsort -ldivsufsort64 -O3

fm: FM-index.cpp
	g++ FM-index.cpp -o fm -lsdsl -ldivsufsort -ldivsufsort64 -O3
	