#Compiler + flags
TARGET	:= pwmbuzz
CC	:= gcc
CFLAGS	:= -I.
SRC	:= pwm.c

begin: $(TARGET)

#Create obj file from src
$(TARGET).o: $(SRC)
	$(CC) $(CFLAGS) -c -o $(TARGET).o $(SRC)

#Create executable from obj file
$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o

#Remove exe and obj files
.PHONY: clean
clean:
	rm -f $(TARGET).o $(TARGET)

