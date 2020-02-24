FILES:=sources/source.cpp sources/Utils.cpp sources/Compilers.cpp sources/RunningSubsystem.cpp
FLAGS:=-std=c++17

build:
	g++ -O3 $(FLAGS) $(FILES) -o run

debug_build:
	g++ -g -no-pie $(FLAGS) $(FILES) -o run	

tests:
	echo examples/* | xargs -n 1 ./run 

build_and_run_tests: build tests
