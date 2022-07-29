cmp_branches.so : cmp_branches.o
	g++ -shared cmp_branches.o -lcurl -o cmp_branches.so `curl-config --libs`

cmp_branches.o : cmp_branches.cpp cmp_branches.hpp
	g++ -c -fPIC cmp_branches.cpp -o cmp_branches.o `curl-config --cflags`