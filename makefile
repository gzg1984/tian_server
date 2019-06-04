OS:=$(shell uname)
ifeq ($(OS),Darwin)
MYSQL_LIB+=$(shell /usr/local/mysql/bin/mysql_config --libs)
MYSQL_HEAD+=$(shell /usr/local/mysql/bin/mysql_config --include)
else
MYSQL_LIB+=$(shell mysql_config --libs)
MYSQL_HEAD+=$(shell mysql_config --include)
endif

TARGET=ServerApp 
all:${TARGET} 

ServerApp:TcpForkMysql.cpp
	g++ $^ -o $@ $(MYSQL_LIB) $(MYSQL_HEAD)

#Only For Mac
run:ServerApp
	DYLD_LIBRARY_PATH=/usr/local/mysql/lib ./ServerApp

clean:
	rm ${TARGET}
	rm *~
