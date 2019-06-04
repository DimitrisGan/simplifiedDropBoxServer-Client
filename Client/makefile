# Declaration of variables
CC = g++
CC_FLAGS = -Wall -w -o  -std=c++11
LDFLAGS = -pthread

# File names
EXEC = dropbox_client
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
 
# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXEC)

# To obtain object files
%.o: %.cpp 
	$(CC)  -c $(CC_FLAGS)  $< -o $@  

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)