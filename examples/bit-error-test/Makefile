TARGET ?= sky

CONTIKI_PROJECT = test
all: $(CONTIKI_PROJECT)

CONTIKI_WITH_RIME=1

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

CONTIKI = ../..
include $(CONTIKI)/Makefile.include
