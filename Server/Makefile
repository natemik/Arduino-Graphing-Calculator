CC = g++
CFLAGS = -c -Wall -std=c++11
LFLAGS = -static
PROGRAM = calculator
OBJS = main.o shunting.o serialport.o

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) $(LFLAGS)

main.o: main.cpp shunting.h serialport.h 
	$(CC) main.cpp -o main.o $(CFLAGS)

shunting.o: shunting.cpp
	$(CC) shunting.cpp -o shunting.o $(CFLAGS)

serialport.o: serialport.cpp serialport.h
	$(CC) serialport.cpp -o serialport.o $(CFLAGS)

clean:
	@rm -f $(OBJS)
	@rm -f $(PROGRAM)