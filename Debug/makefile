################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include tools/teqser/subdir.mk
-include tools/shadeops/sci_volumes/src/subdir.mk
-include tools/ptview/subdir.mk
-include tools/ptbounce/subdir.mk
-include tools/procedurals/hairgen/subdir.mk
-include tools/procedurals/hairgen/kdtree/subdir.mk
-include tools/piqsl/subdir.mk
-include tools/miqser/subdir.mk
-include tools/eqsl/subdir.mk
-include tools/displays/xpm/subdir.mk
-include tools/displays/sdcWin32/subdir.mk
-include tools/displays/sdcBMP/subdir.mk
-include tools/displays/piqsl/subdir.mk
-include tools/displays/file/subdir.mk
-include tools/displays/exr/subdir.mk
-include tools/displays/dspyutil/subdir.mk
-include tools/dbo/plane/subdir.mk
-include tools/aqsltell/subdir.mk
-include tools/aqsl/subdir.mk
-include tools/aqsis/subdir.mk
-include thirdparty/tinyxml/subdir.mk
-include thirdparty/pdiff/subdir.mk
-include thirdparty/partio/src/src/tools/subdir.mk
-include thirdparty/partio/src/src/tests/subdir.mk
-include thirdparty/partio/src/src/lib/io/subdir.mk
-include thirdparty/partio/src/src/lib/core/subdir.mk
-include thirdparty/aqiv/subdir.mk
-include prototypes/shaderVM_codegen/subdir.mk
-include prototypes/sampling/subdir.mk
-include prototypes/ri_cpp/subdir.mk
-include prototypes/piqsl_interface/subdir.mk
-include prototypes/newcore/subdir.mk
-include prototypes/newcore/ustring/subdir.mk
-include prototypes/newcore/tbb/subdir.mk
-include prototypes/newcore/scenes/subdir.mk
-include prototypes/newcore/prototype/subdir.mk
-include prototypes/newcore/experiments/subdir.mk
-include prototypes/subdir.mk
-include prototypes/float_array_view/subdir.mk
-include prototypes/binarytree/subdir.mk
-include prototypes/allocators/subdir.mk
-include libs/util/win32/subdir.mk
-include libs/util/subdir.mk
-include libs/util/posix/subdir.mk
-include libs/tex/pointcloud/subdir.mk
-include libs/tex/maketexture/subdir.mk
-include libs/tex/io/subdir.mk
-include libs/tex/filtering/subdir.mk
-include libs/tex/buffers/subdir.mk
-include libs/slxargs/subdir.mk
-include libs/slcomp/parse/subdir.mk
-include libs/slcomp/backend/subdir.mk
-include libs/shadervm/subdir.mk
-include libs/shadervm/shaderexecenv/subdir.mk
-include libs/riutil/subdir.mk
-include libs/ri2rib/subdir.mk
-include libs/pointrender/nondiffuse/subdir.mk
-include libs/pointrender/subdir.mk
-include libs/pointrender/diffuse/subdir.mk
-include libs/math/subdir.mk
-include libs/core/subdir.mk
-include libs/core/texturing_old/subdir.mk
-include libs/core/raytrace/subdir.mk
-include libs/core/geometry/subdir.mk
-include libs/core/ddmanager/subdir.mk
-include libs/core/api/subdir.mk
-include libs/build_tools/subdir.mk
-include examples/procedurals/menger/subdir.mk
-include doc/guide/dev/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: aqsis-1.8.2

# Tool invocations
aqsis-1.8.2: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o"aqsis-1.8.2" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) aqsis-1.8.2
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
