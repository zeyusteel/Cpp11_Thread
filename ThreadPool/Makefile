all: threadpool
LIBS = -lpthread

threadpool: ThreadPool.cpp main.cpp
	clang++ -g -Wall -std=c++11 $^ $(LIBS) -o $@ 

clean:
	rm threadpool
