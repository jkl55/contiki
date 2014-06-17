TARGET ?= z1

CONTIKI_PROJECT = test
all: $(CONTIKI_PROJECT)

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\" -DDCOSYNCH_CONF_ENABLED=0

CONTIKI = ../..
include $(CONTIKI)/Makefile.include
