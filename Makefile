GPP = g++
FLAGS = -O3 -g
#FLAGS += -DNDEBUG
LDFLAGS = -pthread

PROGRAMS = test_avl
#PROGRAMS = benchmark benchmark_sanitize

all: $(PROGRAMS)

build:
	mkdir -p $@

#benchmark: build
#	$(GPP) $(FLAGS) -MMD -MP -MF build/$@.d -o $@ $@.cpp $(LDFLAGS)
#	
#
#benchmark_sanitize:
#	$(GPP) $(FLAGS) -MMD -MP -MF build/$@.d -o $@ benchmark.cpp $(LDFLAGS) -fsanitize=address -static-libasan
#
	
test_avl: build
	$(GPP) $(FLAGS) -MMD -MP -MF build/$@.d -o $@ test/test_avl.cpp
	
-include $(addprefix build/,$(addsuffix .d, $(PROGRAMS)))

clean:
	rm -rf $(PROGRAMS) build


