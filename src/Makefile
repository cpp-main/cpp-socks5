.PHONY: clean distclean

CXX = g++

EXE = cpp-socks5
OBJECTS = app_main.o
SOCKS5_LIB = ./socks5/libsocks5.a

CXXFLAGS := -I$(HOME)/.tbox/include/
LDFLAGS := -L$(HOME)/.tbox/lib/

LIBS := \
	-ltbox_main \
	-ltbox_coroutine \
	-ltbox_terminal \
	-ltbox_network \
	-ltbox_eventx \
	-ltbox_event \
	-ltbox_log \
	-ltbox_trace \
	-ltbox_util \
	-ltbox_base \
	-lpthread \
	-ldl

all: $(EXE)

$(SOCKS5_LIB):
	make -C socks5 libsocks5.a

$(EXE) : $(OBJECTS) $(SOCKS5_LIB)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS)

distclean: clean
	make -C socks5 distclean
	rm -f $(EXE)
