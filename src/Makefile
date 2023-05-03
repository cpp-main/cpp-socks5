TARGET:=cpp-socks5

OBJECTS:=\
	app_main.o \
	socks5_module.o \
	socks5_session.o

CXXFLAGS:=-I$(HOME)/.tbox/include
LDFLAGS:=-L$(HOME)/.tbox/lib

LIBS:=\
	-ltbox_main \
	-ltbox_terminal \
	-ltbox_network \
	-ltbox_eventx \
	-ltbox_event \
	-ltbox_log \
	-ltbox_util \
	-ltbox_base \
	-lpthread \
	-ldl

CXXFLAGS+=-DLOG_MODULE_ID='"socks5"' -ggdb

#CXXFLAGS+=-fsanitize=address -fno-omit-frame-pointer
#LDFLAGS+=-fsanitize=address -static-libasan

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	g++ -o $@ $^ $(LDFLAGS) $(LIBS) -rdynamic

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm $(TARGET)
