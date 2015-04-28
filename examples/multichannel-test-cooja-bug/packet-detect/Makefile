TARGET ?= z1

CONTIKI_PROJECT = main

all: $(CONTIKI_PROJECT)

# use RIME, not IP
CONTIKI_WITH_RIME=1

ifeq ($(MANY_NODES),1)
CFLAGS += -DMANY_NODES=1
endif

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI = ../..
include $(CONTIKI)/Makefile.include
