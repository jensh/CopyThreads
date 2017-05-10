# (c) 2017-05-10 Jens Hauke <jens.hauke@4k2.de>

all:

CTHREAD_DIR = ../../src

CPPFLAGS += -MMD -I$(CTHREAD_DIR)
CFLAGS   += -Wall -W -Wno-unused-parameter -Wno-unused -g
CFLAGS   += -Wconversion -Wsign-conversion
LDFLAGS  += -g
LDLIBS += $(LDLIBS_$@)

VPATH += $(CTHREAD_DIR)
#########################################################


PROGS += hello_world
hello_world: hello_world.o cthread.o

PROGS += wait_for_condition
wait_for_condition: wait_for_condition.o cthread.o


#########################################################

OBJECTS  += $(wildcard *.o)
DEPFILES := $(OBJECTS:.o=.d)
CLEAN    += $(patsubst .clean.%,%, $(wildcard .clean.*)) $(wildcard .clean.*)
CLEAN    += $(OBJECTS) $(DEPFILES)
MRPROPPER+= $(PROGS) $(wildcard *~)

all: $(PROGS)

clean:
	$(RM) $(CLEAN)

mrpropper distclean:
	$(RM) $(CLEAN) $(MRPROPPER)

-include $(DEPFILES) dummy.d
