CXX = g++
LDFLAGS =
DEFINES =
CFLAGS = -O3 -g -std=c++0x -fno-builtin $(DEFINES)

.PHONY: all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(SDSCXX) -fPIC $(CFLAGS) -c $< -o $@

convertnp2hex.so: convertnp2hex.o
	$(CXX) $(CFLAGS) -fPIC -shared $^ $(LDFLAGS) -o $@

all: convertnp2hex.so
