.PHONY: clean distclean

CXX ?= g++
AR  ?= ar

LIB_NAME = socks5
STATIC_LIB = lib$(LIB_NAME).a
SHARED_LIB = lib$(LIB_NAME).so.0.0.0

OBJECTS = \
	create.o \
	plugin.o \
	module.o \
	session.o \

CXXFLAGS := -I$(HOME)/.tbox/include/ -fPIC -DLOG_MODULE_ID='"socks5"'
LDFLAGS := -L$(HOME)/.tbox/lib/
LIBS := \
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

all: $(STATIC_LIB) $(SHARED_LIB)

$(STATIC_LIB) : $(OBJECTS)
	$(AR) rc $@ $^

$(SHARED_LIB) : $(OBJECTS)
	$(CXX) -shared $^ -Wl,-soname,$(SHARED_LIB) -o $@ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(STATIC_LIB) $(SHARED_LIB)
