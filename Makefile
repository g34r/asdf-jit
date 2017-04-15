CXX = clang++
CXXFLAGS = -std=c++14 -lLLVM -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS
TARGET = a.out
OBJECTS = main.o

all : $(TARGET) 

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
            
clean : 
	rm -rf *.o a.out
