# 聊天室的makefile
SERV_INCLUDES = -I ./inc/server -I ./inc
CLIT_INCLUDES = -I ./inc/client	-I ./inc

CC = gcc
CCFLAGS = -g
LIBS = -lreadline -lmysqlclient -lpthread

# server
SERV_SOURCES = $(wildcard ./src/server/*.c)
SERV_OBJECTS = $(SERV_SOURCES:./src/server/%.c=./obj/%.o)
# client
CLIT_SOURCES = $(wildcard ./src/client/*.c)
CLIT_OBJECTS = $(CLIT_SOURCES:./src/client/%.c=./obj/%.o)

SERV_TARGET = server
CLIT_TARGET = client
TARGET = $(SERV_TARGET) $(CLIT_TARGET)

.PHONY:all clean

all:$(TARGET)
	
$(SERV_OBJECTS):./obj/%.o:./src/server/%.c
	$(CC) $< -c -o $@ $(CCFLAGS) $(SERV_INCLUDES)
$(CLIT_OBJECTS):./obj/%.o:./src/client/%.c
	$(CC) $< -c -o $@ $(CCFLAGS) $(CLIT_INCLUDES)
$(SERV_TARGET):$(SERV_OBJECTS)
	$(CC) $^ -o $(SERV_TARGET) $(CCFLAGS) $(LIBS) $(SERV_INCLUDES)
$(CLIT_TARGET):$(CLIT_OBJECTS)
	$(CC) $^ -o $(CLIT_TARGET) $(CCFLAGS) $(LIBS) $(CLIT_INCLUDES)
clean:
	rm -rf $(SERV_OBJECTS) $(CLIT_OBJECTS) 
	@echo 移除.o文件成功
