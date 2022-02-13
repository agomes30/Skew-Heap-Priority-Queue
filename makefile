CXX = g++ 
CXXFLAGS = -Wall

mytest: jqueue.h jqueue.cpp mytest.cpp
	$(CXX) $(CXXFLAGS) jqueue.h jqueue.cpp mytest.cpp -o mytest
jqueue.o: jqueue.h jqueue.cpp
	$(CXX) $(CXXFLAGS) -c jqueue.cpp
run:
	./mytest
val:
	valgrind ./mytest
