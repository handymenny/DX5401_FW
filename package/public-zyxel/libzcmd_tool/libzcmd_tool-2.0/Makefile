OBJS = zcmd_tool.o
CURRENT_DIR = $(shell pwd)

ifeq ($(CONFIG_ZYXEL_MEMORY_LEAK_DETECT),y)
CFLAGS += -DZCFG_MEMORY_LEAK_DETECT
endif

LDFLAGS += -L$(USRLIBDIR) -ljson -lzos
CFLAGS += -I$(TOOLCHAIN)/include -I. -std=gnu99

.PHONY : libzcmd_tool

all: clean libzcmd_tool

.c.o:
	$(CC) $(CFLAGS) -Wall -fPIC -c $< -o $@

libzcmd_tool : $(OBJS)
	$(CC) $(LDFLAGS) -shared -o libzcmd_tool.so $(OBJS)

clean:
	rm -rf *.o
	rm -rf *.so
