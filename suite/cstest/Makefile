SOURCE = src
INCLUDE = include ../../include
BUILD = build
LIBRARY = -lcapstone -L../..

all:
	rm -rf $(BUILD)
	mkdir $(BUILD)
	$(CC) $(SOURCE)/*.c $(INCLUDE:%=-I %) ${CMAKE_C_FLAGS} -g -o $(BUILD)/cstest $(LIBRARY) $(shell pkg-config --libs cmocka) $(shell pkg-config --cflags cmocka)
cstest:
	$(BUILD)/cstest -d ../MC	
clean:
	rm -rf $(BUILD)
