FILES:=sources/source.cpp sources/Utils.cpp sources/Compilers.cpp sources/RunningSubsystem.cpp
FLAGS:=-std=c++17

build:
	g++ -O3 $(FLAGS) $(FILES) -o ucc

debug_build:
	g++ -g -no-pie $(FLAGS) $(FILES) -o ucc
