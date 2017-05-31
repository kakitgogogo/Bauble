CXXFLAGS	= -g -O0 -Wall -I ../../kakui -I ../.. -pthread
LDFLAGS		= -lpthread -lkakui -L.
BASE_SRC	= $(wildcard ../../kakui/base/*.cpp)
LIB_SRC		= $(wildcard ../../kakui/net/*.cpp)
KAKUI_SRC	= $(notdir $(BASE_SRC) $(LIB_SRC))
OBJS		= $(patsubst %.cpp,%.o,$(KAKUI_SRC))

TESTS = test5 test6

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
