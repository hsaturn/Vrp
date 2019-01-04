HAVE_SYNTH=1
OPT_LEVEL=-g
OPT:=-std=c++17 -Wall ${OPT_LEVEL}

ifeq ($(HAVE_SYNTH),1)
LIBS=-lsynthetizer -lX11
OPT:=${OPT} -DHAVE_SYNTH -g
endif

LIBS:=${LIBS} -lsfml-graphics -lglfw
APPS_FOLDERS:=$(wildcard apps/*)
APPS:=$(APPS_FOLDERS) apps/RedStone/Blocks  apps/Cube/commands apps/world/Blocks
PARTS = core core/model core/widgets core/genetic core/commands $(APPS)
RENDERER:=renderer/opengl
CXX=g++

SRCE:= $(foreach dir,$(PARTS),$(wildcard $(dir)/*.cpp)) \
	$(wildcard $(RENDERER)/*.cpp)

BUILD_DIR=build
OBJS=$(addprefix $(BUILD_DIR)/,$(SRCE:.cpp=.o))
DEPS=$(addprefix $(BUILD_DIR)/,$(SRCE:.cpp=.d))
INC_DIRS=core core/widgets core/genetic ../SaturnLib/include
INCLUDES:=$(addprefix -I,$(INC_DIRS)) -I/usr/include/freetype2

.PHONY:	build_dir

all: build_dir vrp

vrp: $(OBJS)
	@echo "Linking $@"
	$(CXX) $(OPT) $^ -lSDL -lglut -lGL -lGLU -lGLEW $(LIBS) -o $@ -pthread

build_dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o:%.cpp
	@echo "  Compiling ${OPT_LEVEL} $<"
	@mkdir -p $(BUILD_DIR)/$(shell dirname $<)
	@$(CXX) -I$(RENDERER) $(INCLUDES) -MMD -g -Iglm -I/usr/include -I$(shell dirname $<) -I. $(OPT) -c $< -o $@ -pthread

vars:
	echo $(OBJS)

clean:
	@rm -rf $(OBJS)
	@rm -rf $(DEPS)
	@rm -f vrp

-include $(DEPS)
