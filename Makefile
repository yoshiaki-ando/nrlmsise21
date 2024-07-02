

### ===========================================================================

PREFIX = $(HOME)

# suffix for library. Use the versioni of Ubuntu
LIB_SUFFIX =

LIB_DIR = $(PREFIX)/lib
INC_DIR = $(PREFIX)/include
SHARE_DIR = $(PREFIX)/share/msis21

HEADERS = Msis21.h

OBJS = msis_gfn.o msis_utils.o msis_tfn.o msis_dfn.o \
msis_constants.o msis_init.o msis2.1_test.o msis_calc.o \
msis_gtd8d.o wrap_msisinit.o Msis21.o

PARAM = msis21.parm

OPTS = -O3 -Wall -fPIC

TARGET = libnrlmsise21$(LIB_SUFFIX).so

# for test program
LIB_OPTS = -Wl,-R$(LIB_DIR)

.PHONY: all install clean

all: $(TARGET) main

$(TARGET): $(OBJS) $(HEADERS)
	gfortran -shared -o $@ $(OBJS)

install: $(TARGET)
	@if [ ! -d $(LIB_DIR) ]; then \
		mkdir -p $(LIB_DIR); \
	fi
	cp $(TARGET) $(LIB_DIR)
	@if [ ! -d $(INC_DIR) ]; then \
		mkdir -p $(INC_DIR); \
	fi
	cp $(HEADERS) $(INC_DIR)
	@if [ ! -d $(SHARE_DIR) ]; then \
		mkdir -p $(SHARE_DIR); \
	fi
	cp $(

main: nrlmsis21.o $(OBJS)
	g++ -o $@ nrlmsis21.o -lgfortran $(LIB_OPTS) -L$(LIB_DIR) -lnrlmsise21$(LIB_SUFFIX)

%.o: %.f90
	gfortran -cpp -c $< $(OPTS)

%.o: %.cpp Msis21.h
	g++ -c $< -Wall -I$(INC_DIR)

clean:
	rm -rf $(TARGET) main *.o
