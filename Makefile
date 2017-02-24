HAVE_SYNTH=1

OPT=-std=c++0x -g

ifeq ($(HAVE_SYNTH),1)
LIBS=-lsynthetizer
OPT:=${OPT} -DHAVE_SYNTH
endif

LIBS:=${LIBS} -lsfml-graphics

PARTS = core core/model core/widgets Cube Vrp genetic RedStone RedStone/Blocks core/commands Cube/commands world world/Blocks colib
RENDERER:=renderer/opengl
CXX=g++

SRCE:= $(foreach dir,$(PARTS),$(wildcard $(dir)/*.cpp)) \
	$(wildcard $(RENDERER)/*.cpp)

BUILD_DIR=build
OBJS=$(addprefix $(BUILD_DIR)/,$(SRCE:.cpp=.o))
DEPS=$(addprefix $(BUILD_DIR)/,$(SRCE:.cpp=.d))
INC_DIRS=core core/widgets genetic
INCLUDES:=$(addprefix -I,$(INC_DIRS))

.PHONY:	build_dir

all: build_dir vrp

vrp: $(OBJS)
	@echo "Linking $@"
	$(CXX) $(OPT) $^ -lSDL -lglut -lGL -lGLU -lGLEW $(LIBS) -o $@ -pthread

build_dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o:%.cpp
	@echo "  Compiling $< in `pwd`"
	@mkdir -p $(BUILD_DIR)/$(shell dirname $<)
	@$(CXX) -I$(RENDERER) $(INCLUDES) -MMD -g -Iglm -I/usr/include -I$(shell dirname $<) -Wall -I. $(OPT) -c $< -o $@ -pthread

vars:
	echo $(OBJS)

clean:
	@rm -rf $(OBJS)
	@rm -rf $(DEPS)
	@rm -f vrp

-include $(DEPS)