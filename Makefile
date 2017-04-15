CXX = clang++
CXXFLAGS = -std=c++14 -lLLVM -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS
TARGET = a.out
OBJECTS = main.o

all : $(TARGET) 

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

llc :
	llc -filetype=obj output.ll
	$(CXX) output.o -o output

opt :
	opt -S -O3 output.ll -o optimised_output.ll
	llc -filetype=obj optimised_output.ll
	$(CXX) optimised_output.o -o optimised_output
 
clean : 
	rm -rf *.o a.out output optimised_output *.ll
