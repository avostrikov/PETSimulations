ROOTCONFIG   := root-config

ARCH         := $(shell $(ROOTCONFIG) --arch)
PLATFORM     := $(shell $(ROOTCONFIG) --platform)


ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS  := $(shell $(ROOTCONFIG) --ldflags)
ROOTLIBS     := $(shell $(ROOTCONFIG) --libs)
ROOTGLIBS    := $(shell $(ROOTCONFIG) --glibs)
HASTHREAD    := $(shell $(ROOTCONFIG) --has-thread)
ROOTINC      := $(shell $(ROOTCONFIG) --incdir)


CC            = gcc
CXX           = g++
LD            = g++
OutPutOpt     = -o # keep whitespace after "-o"

#
ifeq ($(ARCH),linux)
CXXFLAGS      = -O2 -Wall -fPIC
LDFLAGS       = -O2
SOFLAGS       = -shared
endif

#
ifeq ($(ARCH),linuxx8664gcc)
CXXFLAGS      = -O2 -Wall -fPIC
LDFLAGS       = -O2
SOFLAGS       = -shared
endif

#
ifeq ($(ARCH),win32gcc)
CXXFLAGS      = -O -pipe -Wall -Woverloaded-virtual -I/usr/X11R6/include
LDFLAGS       = -O -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc \
                -L/usr/X11R6/lib
SOFLAGS       = -shared -D_DLL -Wl,--export-all-symbols
endif


CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)



SJYLX_DIR = /home/heejong/sjy

INCLUDES_C = $(SJYLX_DIR)/include 

SYSLIB  = -lnsl  -ldl  -lcrypt  -lm  	
SJYLIB  = -L$(SJYLX_DIR)/lib -lsjy



COMPILE_CC := $(CC) -c  $(INCLUDES_C:%=-I%) -I$(ROOTINC) 


COMPILE_CXX := $(CXX) -c  $(INCLUDES_C:%=-I%) -I$(ROOTINC)



%.o:%.c
	$(COMPILE_CC) $<

%.o:%.cc
	$(COMPILE_CXX) $<


pro = robox

OBJ = $(pro).o



############################################################


all:    $(pro)

clean:
	rm -f *~ *.o 

$(pro): comport.o $(pro).o
	g++ $(LDFLAGS) -o $(pro) $(OBJ) comport.o $(ROOTLIBS) $(SYSLIB) $(SJYLIB)















