OPT=-std=c++0x -g
PARTS = core Cube Vrp genetic RedStone RedStone/Blocks core/commands Cube/commands world world/Blocks
RENDERER:=renderer/opengl
	
SRCE:= \
	$(foreach dir,$(PARTS),$(wildcard $(dir)/*.cpp)) \
	$(wildcard $(RENDERER)/*.cpp)

BUILD_DIR=build
OBJS=$(addprefix $(BUILD_DIR)/,$(SRCE:.cpp=.o))
DEPS=$(addprefix $(BUILD_DIR)/,$(SRCE:.cpp=.d))
	
.PHONY:	build_dir
	
all: build_dir vrp

vrp: $(OBJS)
	@echo "Linking $@"
	@g++ $(OPT) $^ -lSDL -lglut -lGL -lGLU -lGLEW -o $@ -pthread

build_dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o:%.cpp Makefile
	@echo "  Compiling $<"
	@mkdir -p $(BUILD_DIR)/$(shell dirname $<)
	@g++ -I$(RENDERER) -Icore -Igenetic -MMD -g -Iglm -I/usr/include -I$(shell dirname $<) -Wall -I. $(OPT) -c $< -o $@ -pthread

vars:
	echo $(OBJS)
	
clean:
	@rm -rf $(OBJS)
	@rm -rf $(DEPS)
	@rm -f vrp

-include $(DEPS)

# build tests
build-tests: .build-tests-post

.build-tests-pre:
# Add your pre 'build-tests' code here...

.build-tests-post: .build-tests-impl
# Add your post 'build-tests' code here...


# run tests
test: .test-post

.test-pre:
# Add your pre 'test' code here...

.test-post: .test-impl
# Add your post 'test' code here...
