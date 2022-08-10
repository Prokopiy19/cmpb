bin/prog : bin/main.o bin/shared/libcmpb.so
	g++ bin/main.o -Lbin/shared -lcmpb -o bin/prog

bin/main.o : src/main.cpp
	g++ -c src/main.cpp -o bin/main.o

bin/shared/libcmpb.so : bin/shared/cmpb.o
	g++ -shared bin/shared/cmpb.o -lcurl -o bin/shared/libcmpb.so `curl-config --libs`

bin/shared/cmpb.o : src/cmpb/cmpb.cpp src/cmpb/cmpb.hpp
	g++ -c -fPIC src/cmpb/cmpb.cpp -o bin/shared/cmpb.o `curl-config --cflags`

.PHONY: install
install:
	sudo cp bin/shared/libcmpb.so /usr/lib
	sudo chmod 755 /usr/lib/libcmpb.so

.PHONY: uninstall
uninstall:
	sudo rm /usr/lib/libcmpb.so