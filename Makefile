all: sa fm

sa: sa.cpp
	g++ sa.cpp -o sa -lsdsl -ldivsufsort -ldivsufsort64 -O3

fm: fm.cpp
	g++ fm.cpp -o fm -lsdsl -ldivsufsort -ldivsufsort64 -O3
	