CFLAGS = -std=c99 -Wall -g

DIR_BUILD = ./build

PPFLAGS = -MT $@ -MMD -MP -MF $(DIR_BUILD)/$*.d

SOURCES = $(notdir $(wildcard *.c))

OBJS = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(SOURCES)))

TARGET = test

DEPFILES = $(patsubst %.o, %.d, $(OBJS))

# core header
#vpath %.h ../../inc/core
#vpath %.h ../../inc/toolkit

.PHONY : all output clean
all : $(DIR_BUILD)/$(TARGET)

$(DIR_BUILD)/$(TARGET) : $(OBJS) output
	cc $(CFLAGS) -o $@ $(OBJS)

$(DIR_BUILD)/%.o : %.c $(DIR_BUILD)/%.d output
	cc $(PPFLAGS) $(CFLAGS) -c $< -o $@

$(DIR_BUILD)/%.d : ;
.PRECIOUS : $(DIR_BUILD)/%.d

#create build directory
output : 
	mkdir -p $(DIR_BUILD)

#clean outputs
clean : 
	rm -rf $(DIR_BUILD)

-include $(DEPFILES)

