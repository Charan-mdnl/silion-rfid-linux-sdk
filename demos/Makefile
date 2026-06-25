#wrote by hexp

C := gcc
i=64

ifeq ($(i), 32)
	CFLAGS = -m32 -I./lib -c -g -O2 -Wall
	EXEFLAGS = -m32 -L./lib  -lModuleAPI  -lpthread
else
	CFLAGS = -m64 -I../lib -c -g -O2 -Wall
	EXEFLAGS = -m64 -L./lib  -lModuleAPI  -lpthread
endif

SOURCE_DIR = $(shell pwd)
CFILE_PATTERN = *.c
CFILE = $(shell cd $(SOURCE_DIR); ls $(CFILE_PATTERN))
OBJS =$(patsubst %.c, %.o, $(CFILE))
APPReadTags = ReadTags

$(APPReadTags):$(OBJS)
	rm -f $(APPReadTags)
	$(C) -o  $@  $^ $(EXEFLAGS)


%.o : %.c
	$(C) $(CFLAGS) $< -o $@

.PHONY : clean

clean:
	rm -f *.o $(APPReadTags)

