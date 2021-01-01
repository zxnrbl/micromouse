CXX := g++
INCLUDES := ./mms-cpp/
CXXFLAGS := -Wall -Wextra -O3 -std=c++1z -I$(INCLUDES)
DEBUGFLAGS := -ggdb -fno-eliminate-unused-debug-symbols -Og -std=c++1z -I$(INCLUDES)
BUILD := ./build
RELEASE := $(BUILD)/release
DEBUG := $(BUILD)/debug
REL_OBJ := $(RELEASE)/obj
DEB_OBJ := $(DEBUG)/obj
SRC := $(wildcard *.cpp)
REL_OBJS := $(SRC:%.cpp=$(REL_OBJ)/%.o)
DEB_OBJS := $(SRC:%.cpp=$(DEB_OBJ)/%.o)

release: release_dir $(RELEASE)/mouse

debug: debug_dir $(DEBUG)/mouse

$(REL_OBJ)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DEB_OBJ)/%.o: %.cpp
	$(CXX) $(DEBUGFLAGS) -c $< -o $@

$(RELEASE)/mouse: $(REL_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(DEBUG)/mouse: $(DEB_OBJS)
	$(CXX) $(DEBUGFLAGS) -o $@ $^

release_dir:
	[ -d $(REL_OBJ) ] || mkdir -p $(REL_OBJ)

debug_dir:
	[ -d $(DEB_OBJ) ] || mkdir -p $(DEB_OBJ)

clean:
	rm -rvf $(BUILD)/*

