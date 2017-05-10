# (c) 2017-05-10 Jens Hauke <jens.hauke@4k2.de>

all:

CTHREAD_DIR = ../../src

CPPFLAGS += -MMD -I$(CTHREAD_DIR)
CFLAGS   += -Wall -W -Wno-unused-parameter -Wno-unused -g
LDFLAGS  += -g
LDLIBS += $(LDLIBS_$@)

VPATH += $(CTHREAD_DIR)
#########################################################


PROGS += hello_world
hello_world: hello_world.o cthread.o

test: test@cprogram

#########################################################
# test@prog: diff `prog <prog.in` with prog.out
%.in:
	touch $@ #< empty input

test@%: % %.in
	@touch .clean.$*.output
	$(abspath $<) < $*.in > $*.output
	diff -U 5 -p $*.output $*.out
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
