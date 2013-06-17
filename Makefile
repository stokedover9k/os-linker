
EXECUTABLE = linker

INCLUDE_DIRS := 
LIB_DIRS := 
LIBS := 

CXXFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))
CXXFLAGS += -std=c++0x
CXXFLAGS += -DFILELOG_MAX_LEVEL=$(FILELOG_MAX_LVL)
LDFLAGS += $(foreach librarydir,$(LIB_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(LIBS),-l$(library))

FILELOG_MAX_LVL=logDEBUG2

C_SRCS             := 
CXX_SRCS           := main.cpp parser.cpp parse-error.cpp
GENERATED_C_SRCS   := 
GENERATED_CXX_SRCS := 

C_OBJS   := ${C_SRCS:.c=.o} ${GENERATED_C_SRCS:.c=.o}
CXX_OBJS := ${CXX_SRCS:.cpp=.o} ${GENERATED_CXX_SRCS:.cpp=.o}
OBJS     := ${C_OBJS} ${CXX_OBJS}

.PHONY: all clean

all: ${EXECUTABLE} 

${EXECUTABLE}: ${OBJS}
	$(LINK.cc) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)

clean:
	rm -fv ${OBJS} ${EXECUTABLE}

zipdevel:
	zip linker.devel.zip $(CXX_SRCS) main.h parser.h parse-error.h test.sh inout/* README.md lab1-linker.pdf

define OBJECT_DEPENDS_ON_CORRESPONDING_HEADER
        $(1) : ${1:.o=.h}
endef

$(foreach object_file,$(OBJS),$(eval $(call OBJECT_DEPENDS_ON_CORRESPONDING_HEADER,$(object_file))))