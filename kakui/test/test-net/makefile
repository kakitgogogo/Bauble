CXXFLAGS	= -g -O0 -Wall -I ../../kakui -I ../.. -pthread
LDFLAGS		= -lpthread -lkakui -L.
BASE_SRC	= $(wildcard ../../kakui/base/*.cpp)
LIB_SRC		= $(wildcard ../../kakui/net/*.cpp)
KAKUI_SRC	= $(notdir $(BASE_SRC) $(LIB_SRC))
OBJS		= $(patsubst %.cpp,%.o,$(KAKUI_SRC))

TESTS = test5 test6 test7 test8 test9 test11 test12

all: $(TESTS)
	rm -f *.o

libkakui.a: $(BASE_SRC) $(LIB_SRC)
	g++ $(CXXFLAGS) -c $^
	ar rcs $@ $(OBJS)

$(TESTS): libkakui.a
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TESTS) *.o *.a


test5: test5.cpp
test6: test6.cpp
test7: test7.cpp
test8: test8.cpp
test9: test9.cpp
test11: test11.cpp
test12: test12.cpp