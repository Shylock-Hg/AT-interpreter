CC = gcc

CFLAGS = -std=c99 -Wall

DIR_BUILD = ./build

PPFLAGS = -MT $@ -MMD -MP -MF $(DIR_BUILD)/$*.d

SOURCES = src/at_command.c \
		  src/at_fsm.c \
		  src/at_xrecord.c \
		  src/hash.c \
		  src/queue.c \
		  src/stdlog.c \
		  test.c

OBJECTS = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(notdir $(SOURCES))))

TARGET = test

DEPFILES = $(patsubst %.o, %.d, $(OBJECTS))

# set c sources search path
vpath %.c $(sort $(dir $(SOURCES)))

.PHONY : all clean
all : $(DIR_BUILD)/$(TARGET)

$(DIR_BUILD)/$(TARGET) : $(OBJECTS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

$(DIR_BUILD)/%.o : %.c Makefile | $(DIR_BUILD)
	$(CC) $(PPFLAGS) $(CFLAGS) -c $< -o $@

#$(OBJECTS): $(DIR_BUILD)/%.o: %.c
	#mkdir -p $(@D)
	#$(CC) $(PPFLAGS) $(CFLAGS) -c $< -o $@

$(DIR_BUILD)/%.d : ;
.PRECIOUS : $(DIR_BUILD)/%.d

$(DIR_BUILD) : 
	mkdir -p $(DIR_BUILD)

clean : 
	rm -rf $(DIR_BUILD)

-include $(DEPFILES)

