LIBRARY_DIRECTORIES = scheduler physical_robot # robot_library

LIBRARY_SOURCE_DIRECTORIES = $(patsubst %, %/src, $(LIBRARY_DIRECTORIES))
LIBRARY_INCLUDE_DIRECTORIES = $(patsubst %, %/include, $(LIBRARY_DIRECTORIES))

SOURCE_FILES = $(foreach dir, $(LIBRARY_SOURCE_DIRECTORIES), $(wildcard $(dir)/*.cc) $(wildcard $(dir)/**/*.cc))
SOURCE_FILES += main/src/gmr_main.cc
OBJECT_FILES = $(patsubst %/src/%.cc, %/build/%.o, $(SOURCE_FILES))

$(info LIBRARY_SOURCE_DIRECTORIES: $(LIBRARY_SOURCE_DIRECTORIES))
$(info SOURCE_FILES: $(SOURCE_FILES))

CXX = arm-linux-gnueabi-g++
CPPFLAGS = -c -Wall -I. -MD -MP $(addprefix -I, $(LIBRARY_INCLUDE_DIRECTORIES))
LDFLAGS = lib/gmr_libev3dev.a

BUILD_DIRECTORIES = $(patsubst %/src, %/build, $(LIBRARY_SOURCE_DIRECTORIES))

$(BUILD_DIRECTORIES):
	mkdir -p $@

all: $(OBJECT_FILES)
	$(CXX) -o main_build $(OBJECT_FILES) $(LDFLAGS)

# Pattern rule to compile .cc files to .o files
%/build/%.o: %/src/%.cc | $(BUILD_DIRECTORIES)
	$(CXX) $(CPPFLAGS) -c $< -o $@
	$(CXX) $(CPPFLAGS) -MM $< > $(@:.o=.d)

# Explicit rule for main.cpp
/build/main.o: main/src/gmr_main.cc | $(BUILD_DIRECTORIES)
	$(CXX) $(CPPFLAGS) -c $< -o $@

-include $(patsubst %, %/build/%.d, $(SOURCE_FILES))

.PHONY: clean
clean:
	rm -rf $(patsubst %, %/build, $(LIBRARY_DIRECTORIES))/*.o
	rm -rf $(patsubst %, %/build, $(LIBRARY_DIRECTORIES))/*.d
	rm -rf /build/main.o /build/main.d