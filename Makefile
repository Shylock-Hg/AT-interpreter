#CC = gcc
LN = ln
INSTALL = install

CFLAGS_LOCAL = -Wall -g -std=gnu99 -coverage
CFLAGS_LOCAL += ${CFLAGS}

prefix = /usr/local

DIR_BUILD = build

PPFLAGS = -MT $@ -MMD -MP -MF $(DIR_BUILD)/$*.d

LIB_SOURCES = src/at_command.c \
		  src/at_fsm.c \
		  src/at_xrecord.c \
		  src/hash.c \
		  src/queue.c \
		  src/stdlog.c \
		  src/at_table.c \
		  src/at_param.c

APP_SOURCE = test.c

LIB_OBJECTS = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(notdir $(LIB_SOURCES))))

APP_OBJECT = test.o

LIBVERSION = 0.0.1
LIBNAME = at

LIB_SO_AT = lib$(LIBNAME).so.$(LIBVERSION)

TARGET = at

DEPFILES = $(patsubst %.o, %.d, $(LIB_OBJECTS) $(APP_OBJECT))

# set c sources search path
vpath %.c $(sort $(dir $(LIB_SOURCES)))

.PHONY : all clean install uninstall test
all : $(DIR_BUILD)/$(TARGET)

$(DIR_BUILD)/$(TARGET) : $(DIR_BUILD)/$(APP_OBJECT) $(DIR_BUILD)/$(LIB_SO_AT) Makefile
	$(CC) $(CFLAGS_LOCAL) -o $@ $< -L$(shell pwd)/$(DIR_BUILD) -lat

$(DIR_BUILD)/$(LIB_SO_AT) : $(LIB_OBJECTS) Makefile
	$(CC) $(CFLAGS_LOCAL) -shared -o $@ $(LIB_OBJECTS)
	$(LN) -sf $(shell pwd)/$(DIR_BUILD)/$(LIB_SO_AT) $(DIR_BUILD)/lib$(LIBNAME).so

$(DIR_BUILD)/$(APP_OBJECT) : $(APP_SOURCE) Makefile | $(DIR_BUILD)
	$(CC) -MT $@ -MMD -MP -MF $*.d $(CFLAGS_LOCAL) -c $< -o $@

$(DIR_BUILD)/%.o : %.c Makefile | $(DIR_BUILD)
	$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -fPIC -c $< -o $@

#$(OBJECTS): $(DIR_BUILD)/%.o: %.c
	#mkdir -p $(@D)
	#$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -c $< -o $@

$(DIR_BUILD)/%.d : ;
.PRECIOUS : $(DIR_BUILD)/%.d

$(DIR_BUILD) : 
	mkdir -p $(DIR_BUILD)

install : all
	$(INSTALL) -d "${DESTDIR}${prefix}/lib"
	$(INSTALL) $(DIR_BUILD)/$(LIB_SO_AT) "${DESTDIR}${prefix}/lib"
	$(LN) -sf ${DESTDIR}${prefix}/lib/$(LIB_SO_AT) ${DESTDIR}${prefix}/lib/lib$(LIBNAME).so
	$(INSTALL) -d "${DESTDIR}${prefix}/bin"
	$(INSTALL) $(DIR_BUILD)/$(TARGET) "${DESTDIR}${prefix}/bin"

uninstall : 
	rm -f "${DESTDIR}${prefix}/lib/$(LIB_SO_AT)"
	rm -f "${DESTDIR}${prefix}/lib/lib$(LIBNAME).so"
	rm -f "${DESTDIR}${prefix}/bin/$(TARGET)"

test : ${DESTDIR}/${prefix}/bin/$(TARGET) ${DESTDIR}${prefix}/lib/lib$(LIBNAME).so
	at test.at > log && diff log stdlog

clean : 
	rm -rf $(DIR_BUILD)

-include $(DEPFILES)

