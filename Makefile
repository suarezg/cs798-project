GPP = g++
FLAGS = -O0 -g
#FLAGS += -DNDEBUG
LDFLAGS = -pthread

#PROGRAMS = test_avl
PROGRAMS = benchmark benchmark_debug benchmark_sanitize test_avl test_linkedlist test_redblack

all: $(PROGRAMS)

build:
	mkdir -p $@

benchmark: build
	$(GPP) $(FLAGS) -MMD -MP -MF build/$@.d -o $@ test/$@.cpp $(LDFLAGS)
	

benchmark_sanitize:
	$(GPP) $(FLAGS) -MMD -MP -MF build/$@.d -o $@ test/benchmark.cpp $(LDFLAGS) -fsanitize=address -static-libasan

benchmark_debug:
	$(GPP) $(FLAGS) -o $@.out test/benchmark.cpp -DTRACE=if\(1\) $(LDFLAGS)

	
test_avl: build
	$(GPP) $(FLAGS) -MMD -MP -MF build/$@.d -o $@ test/test_avl.cpp 

test_linkedlist: build
	$(GPP) $(FLAGS) -MMD -MP -MF build/$@.d -o $@ test/test_linkedlist.cpp 
	
test_redblack: build
	$(GPP) $(FLAGS) -MMD -MP -MF build/$@.d -o $@ test/test_redblack.cpp 
	
-include $(addprefix build/,$(addsuffix .d, $(PROGRAMS)))

clean:
	rm -rf $(PROGRAMS) build


