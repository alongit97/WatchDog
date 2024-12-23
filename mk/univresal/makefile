TARGET = none
WS =

# F - Target's folder
# I - Additional Includes' folder
# LF - Additional Linker's flags

F = utils
LF = -pthread
I =

WORKSPACE = ~/git/
SRC = $(WORKSPACE)${F}/src/
INC = $(WORKSPACE)${F}/inc/
TST = $(WORKSPACE)${F}/test/

CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -fPIC -I $(INC) -I $(WORKSPACE)${I}
LDFLAGS = -L. $(addprefix -l,$(patsubst %.o, %, $(DEPENDENCIES_OBJ))) $(LF) -Wl,-rpath=.


ifneq ($(TARGET),none)
DEPENDENCIES = $(shell $(CC) -MM $(SRC)$(TARGET).c $(CFLAGS))
DEPENDENCIES_SRC = $(shell echo $(subst $(TARGET).o: ,,$(patsubst %.h, %.c,$(subst inc,src,$(DEPENDENCIES)))))
DEPENDENCIES_OBJ = $(shell basename -a $(patsubst %.c, %.o,$(DEPENDENCIES_SRC)))
endif

DEBUG = 1
SUBLIB = 0

.PHONY: default clean cleano cleanall release debug all test help

ifeq ($(TARGET),none)
default:
	@echo invalid option
	@echo try \'make help\' for more information.
endif
ifeq ($(SUBLIB), 0)
$(TARGET).out: lib$(TARGET).so $(TARGET)_test.o
	@$(CC) $(TARGET)_test.o -o $@ $(LDFLAGS)
	@echo $@ Compiled Successfuly!
endif

lib$(TARGET).so: $(DEPENDENCIES_OBJ)
	@$(CC) -shared -o $@ $< $(LDFLAGS)
	@echo $@ Library File Created!
						
$(TARGET).o: $(SRC)$(TARGET).c
ifeq ($(DEBUG), 1)
	@$(CC) -c $(SRC)$(TARGET).c -g $(CFLAGS) 
else
	@$(CC) -c $(SRC)$(TARGET).c -NDEBUG -O3 $(CFLAGS) 
endif
	@echo $@ Object File Created!
	
$(TARGET)_test.o: $(TST)$(TARGET)_test.c
	@$(CC) -c $(TST)$(TARGET)_test.c -g $(CFLAGS) 

%.o: 
	@make -s TARGET=$(patsubst %.o,%,$@) F=ds SUBLIB=1

cleanall:
	@rm *.o *.so *.out -f
	@echo All files Removed Successfuly!
	
clean:
	@rm *.o *.so -f 
	@echo Object files and libraries Removed Successfuly!

cleano:
	@rm *.o -f
	@echo All Object files Removed Successfuly!

release: 
	@make -s TARGET=$(TARGET) DEBUG=0

debug: $(TARGET).out

all: release lib$(TARGET).so
	@make -s cleano

init:
	@sed -i '12s|WORKSPACE = .*|WORKSPACE = $(WS)|' Makefile

help:
	@echo Ver. 1.1.2
	@echo 
	@echo :::FIRST USE:::
	@echo On your first use, you will have to initiallize your WORKSPACE root directory.
	@echo to do so use init command as follows:
	@echo \'make init WS=full_root_path\'
	@echo 
	@echo default compilation folder is \'WORKSPACE/ds/\'
	@echo make sure your compilation folder contains at least:
	@echo /src/ - for your source file \(\'target.c\'\)
	@echo /inc/ - for your main header file \(\'target.h\'\)
	@echo /test/ - for your test file \(\'target_test.c\'\)
	@echo 
	@echo if you want to use additional header files from addional folder use I flag.
	@echo 
	@echo 
	@echo :::FLAGS:::
	@echo TARGET - Compilation target.
	@echo WORKSPACE - Root folder.
	@echo WS - Initiallized root folder \(use only when initiallizing your root directory\)
	@echo F - Path from WORKSPACE to the compilation folder. \(\'ds/\' by default\).
	@echo I - Path from WORKSPACE to an additional headers folder.
	@echo LF - Additional flags to the linker.
	@echo ---------------------------------------------
	@echo 
	@echo Examples for usage:
	@echo ---------------------------------------------
	@echo \"make TARGET=myfile\"
	@echo 
	@echo In the above example make will create myfile.out using
	@echo 
	@echo WORKSPACE/ds/src/myfile.c
	@echo WORKSPACE/ds/inc/myfile.h
	@echo WORKSPACE/ds/test/myfile_test.c
	@echo ---------------------------------------------
	@echo 
	@echo \"make TARGET=myfile F=projects I=ds/inc LF=-lm\"
	@echo 
	@echo In the above example make will create myfile.out using
	@echo 
	@echo WORKSPACE/projects/src/myfile.c
	@echo WORKSPACE/projects/inc/myfile.h
	@echo WORKSPACE/projects/test/myfile_test.c
	@echo WORKSPACE/ds/inc/additional-header.h
	@echo WORKSPACE/ds/inc/additional-header.h
	@echo 
	@echo with the additional -lm flag to link math.h header aswell.
	@echo ---------------------------------------------
	@echo 
	@echo 
	@echo :::COMMANDS:::
	@echo make help - User manual.
	@echo make init - Initiallize working directory.
	@echo make cleano - Removes all object files.
	@echo make clean - Removes all objects file and shared object files.
	@echo make cleanall - Removes all files including execute files.
	@echo make release - Compile target in release version.
	@echo make debug - Compile target in debug version \(default option\).






