## Palm OS Protein Generic Makefile Engine for Palm OS Developer Suite v1.1.0
#
# Last edit: 11/10/04
#
# This makefile engine is capable of compiling Protein 
# applications for Palm OS.
#
# This makefile engine assumes that the following variables are 
# set outside of this makefile by a referencing master makefile
# (see a master makefile generated by a Palm OS Developer 
# Suite wizard for detailed explanation of each variable):
#
#	SOURCES
#	RESOURCES
#	DATABASE_RESET 
#	DATABASE_BACKUP 
#	DATABASE_HIDDEN 
#	DATABASE_PROTECT
#	DATABASE_BUNDLE
#	DATABASE_VERSION 
#	DATABASE_NAME 
#	DEBUG_OR_RELEASE 
#	OPTIMIZE_LEVEL 
#	WARNING_LEVEL 
#	ENABLE_EXCEPTIONS
#	ENABLE_ASSERTIONS
#	ADDITIONAL_LINK_LIBRARIES 
#	ADDITIONAL_SIM_LINK_LIBRARIES 
#	LOCAL_INCLUDE_PATHS 
#	LOCAL_SIM_INCLUDE_PATHS 
#	SYSTEM_INCLUDE_PATHS 
#	SYSTEM_SIM_INCLUDE_PATHS 
#	DEFINES 
#	SIM_DEFINES 
#	ADDITIONAL_COMPILER_FLAGS 
#	ADDITIONAL_SIM_COMPILER_FLAGS 
#	ADDITIONAL_LINKER_FLAGS 
#	ADDITIONAL_SIM_LINKER_FLAGS 
#	ADDITIONAL_AR_FLAGS
#	ADDITIONAL_SIM_AR_FLAGS
#	TEXTENCODING
#	LOCALE_CHECKING
#	STRICT_ENCODING
#	TARGET_PLATFORM
#
# Additionally, the user of this makefile engine may obtain the 
# dynamic values for:
#	PROJECT_TYPE
#	SDK_LOCATION
#	SDK_VERSION
#	TOOLS_DIR 
# by doing an optional include of a makefile segment that is 
# generated during the Palm OS Developer Suite standard make build.
# The makefile line to accomplish this is:
#	-include auto-generated.mk
# All master makefiles generated by Palm OS Developer Suite
# wizards include this line already to pick up and use these
# dynamic definitions.
#
# All variable values may be overridden by editing the make command
# for the project (Project->Properties->C/C++ Make Project->Make Builder,
# Build Command field).
#

###############################################################################
# Defaults

comma:= ,

ifeq ($(TOOLS_DIR), )
	TOOLS_DIR =
endif

# Locale (defaults to English)
ifeq ($(LOCALE), )
	LOCALE := enUS
endif

ifeq ($(CREATOR_ID), )
	CREATOR_ID = STRT
endif

ifeq ($(DB_TYPE), )
	DB_TYPE = appl
endif

ifeq ($(SLD_FILE), )
	SLD_FILE = none
endif

ifeq ($(DEBUG_DEVICE_OUTPUT_DIR), )
	DEBUG_DEVICE_OUTPUT_DIR = DebugDevice
endif

ifeq ($(RELEASE_DEVICE_OUTPUT_DIR), )
	RELEASE_DEVICE_OUTPUT_DIR = ReleaseDevice
endif

ifeq ($(DEBUG_SIMULATOR_OUTPUT_DIR), )
	DEBUG_SIMULATOR_OUTPUT_DIR = DebugSim
endif

ifeq ($(RELEASE_SIMULATOR_OUTPUT_DIR), )
	RELEASE_SIMULATOR_OUTPUT_DIR = ReleaseSim
endif

ifeq ($(SDK_VERSION), )
	SDK_VERSION = sdk-6
endif

ifeq ($(ENABLE_EXCEPTIONS), )
	ENABLE_EXCEPTIONS=TRUE
endif

###############################################################################
# App/PRC/Database Names
#
# The difference between App Name and Database name is that App Name may 
# contain spaces while the database name is the App name with spaces removed.
# (Coded in such a way that if the user already explicitly defined these
# variables previously, we'll go with those).

ifeq ($(DATABASE_NAME), )
ifeq ($(ESCAPED_ARTIFACT_NAME), )
	# Database name defaults if ARTIFACT_NAME is empty
	DATABASE_NAME = StartApp
else
	DATABASE_NAME = $(ESCAPED_ARTIFACT_NAME)
endif
endif

ifeq ($(PRC_NAME), )
ifeq ($(ESCAPED_ARTIFACT_NAME), )
	# PRC name defaults if ESCAPED_ARTIFACT_NAME is empty
	PRC_NAME = Start.prc
else
	PRC_NAME = $(ESCAPED_ARTIFACT_NAME).prc
endif
endif

ifeq ($(LIB_NAME), )
ifeq ($(ESCAPED_ARTIFACT_NAME), )
	# LIB name defaults if ESCAPED_ARTIFACT_NAME is empty
	LIB_NAME = Start.a
else
	LIB_NAME = $(ESCAPED_ARTIFACT_NAME).a
endif
endif

ifeq ($(SIM_LIB_NAME), )
ifeq ($(ESCAPED_ARTIFACT_NAME), )
	# SIM LIB name defaults if ESCAPED_ARTIFACT_NAME is empty
	SIM_LIB_NAME = Start.lib
else
	SIM_LIB_NAME = $(ESCAPED_ARTIFACT_NAME).lib
endif
endif

###############################################################################
# Compile and Link Settings

TARGET_FORMAT=PalmOS6

# Compiler settings... compiler + optimizations + debug
# This is a makefile for Palm OS 6 so the compilers used are as follows:
# Device target compiler is pacc
# Simulator target compiler is gcc

CC = "$(TOOLS_DIR)pacc"
SIM_CC = gcc

CFLAGS += -D__PALMOS_KERNEL__=1 -D__PALMOS__=0x06000000
SIM_CFLAGS += -funsigned-char -fexceptions -mno-cygwin -mwindows -D__PALMOS_KERNEL__=1 -D__PALMOS__=0x06000000 -D_SUPPORTS_NAMESPACE=0 -D_SUPPORTS_RTTI=1 -DTARGET_PLATFORM=TARGET_PLATFORM_PALMSIM_WIN32 -DTARGET_HOST=TARGET_HOST_PALMOS
DEPFLAGS = -M1
SIM_DEPFLAGS = -MM -MG -P -w 

# Warning level for device target
ifeq ($(WARNING_LEVEL), ALL)
	CFLAGS += -W4
else
ifeq ($(WARNING_LEVEL), SOME)
	CFLAGS += -W2
else
ifeq ($(WARNING_LEVEL), NONE)
	CFLAGS += -W0
endif
endif
endif

# Warning level for simulator target
ifeq ($(SIM_WARNING_LEVEL), ALL)
	SIM_CFLAGS += -Wall
else
ifeq ($(SIM_WARNING_LEVEL), NONE)
	SIM_CFLAGS += -w
endif
endif

# Warnings as errors for device builds
ifeq ($(WARNING_AS_ERROR), TRUE)
	CFLAGS += -Werror
endif

# Warnings as errors for simulator builds
ifeq ($(SIM_WARNING_AS_ERROR), TRUE)
	SIM_CFLAGS += -Werror
endif

# Adding after other warnings flags so this is always in effect
SIM_CFLAGS += -Wno-multichar 

# Verbose output for device build
ifeq ($(VERBOSE), TRUE)
	CFLAGS += -v
endif

# Verbose output for simulator build
ifeq ($(SIM_VERBOSE), TRUE)
	SIM_CFLAGS += -v
endif

# Dislay logo
ifeq ($(ARM_LOGO), TRUE)
	CFLAGS += -logo
else
	CFLAGS += -nologo
endif

# Exception handling support
ifeq ($(ENABLE_EXCEPTIONS), TRUE)
	CFLAGS += -ex
	LDFLAGS += -ex
endif

# Assertion handling support
ifeq ($(ENABLE_ASSERTIONS), TRUE)
	CFLAGS += -UNDEBUG
else
	CFLAGS += -DNDEBUG=1
endif

# Additional linker flags
ifdef ADDITIONAL_PALINK_FLAGS
	LDFLAGS += $(ADDITIONAL_PALINK_FLAGS)
endif

# Optimization settings
ifeq ($(OPTIMIZE_LEVEL), INLINING)
	OPTIMIZER_FLAG	= -O5
	SIM_OPTIMIZER_FLAG = -O3
else
ifeq ($(OPTIMIZE_LEVEL), INTERPROCEDURAL)
	OPTIMIZER_FLAG	= -O4
	SIM_OPTIMIZER_FLAG = -O3
else
ifeq ($(OPTIMIZE_LEVEL), FULL)
	OPTIMIZER_FLAG	= -O3
	SIM_OPTIMIZER_FLAG = -O3
else
ifeq ($(OPTIMIZE_LEVEL), INTRAPROCEDURAL)
	OPTIMIZER_FLAG	= -O2
	SIM_OPTIMIZER_FLAG = -O3
else
ifeq ($(OPTIMIZE_LEVEL), SOME)
	OPTIMIZER_FLAG	= -O1
	SIM_OPTIMIZER_FLAG = -O1
else
ifeq ($(OPTIMIZE_LEVEL), NONE)
	OPTIMIZER_FLAG	= -O0
	SIM_OPTIMIZER_FLAG = -O0
else
# Default to 0 for debug, 3 for release
ifeq ($(DEBUG_OR_RELEASE), Debug)
	OPTIMIZER_FLAG = -O0
	SIM_OPTIMIZER_FLAG = -O0
else
	OPTIMIZER_FLAG	= -O3
	SIM_OPTIMIZER_FLAG = -O3
endif	
endif
endif
endif
endif
endif
endif
	
# Debug settings (can override optimize settings)
ifeq ($(DEBUG_OR_RELEASE), Debug)
	DEBUG_FLAG += -g
	BUILD_TYPE_FLAG = -DBUILD_TYPE=BUILD_TYPE_DEBUG
	TRACE_OUTPUT_FLAG = -DTRACE_OUTPUT=TRACE_OUTPUT_ON
else
	BUILD_TYPE_FLAG = -DBUILD_TYPE=BUILD_TYPE_RELEASE
	TRACE_OUTPUT_FLAG = -DTRACE_OUTPUT=TRACE_OUTPUT_OFF
endif	

ifeq ($(DEBUG_OR_RELEASE), Debug)
	OBJ_DIR = $(DEBUG_DEVICE_OUTPUT_DIR)
	SIM_OBJ_DIR = $(DEBUG_SIMULATOR_OUTPUT_DIR)
else
	OBJ_DIR = $(RELEASE_DEVICE_OUTPUT_DIR)
	SIM_OBJ_DIR = $(RELEASE_SIMULATOR_OUTPUT_DIR)
endif


CFLAGS += $(OPTIMIZER_FLAG) $(DEBUG_FLAG) $(BUILD_TYPE_FLAG) $(TRACE_OUTPUT_FLAG)
SIM_CFLAGS += $(SIM_OPTIMIZER_FLAG) $(DEBUG_FLAG) $(BUILD_TYPE_FLAG) $(TRACE_OUTPUT_FLAG)

# Linker settings (must come after setting DEBUG_FLAG)
LD = "$(TOOLS_DIR)pacc"
SIM_LD = gcc

LDFLAGS += $(DEBUG_FLAG) -nologo -Wl,-nolocals
SIM_LDFLAGS += $(DEBUG_FLAG) -mno-cygwin -mwindows $(SIM_OBJ_DIR)/gcc_link.def -shared -nostdlib -u___divdi3 -u___moddi3 -u___udivdi3 -u___umoddi3
SIM_LIBS = -L "$(TOOLS_DIR)misclibs" -lpxstlport -lpxsupc++ -lpxgcc -lgcc
ifeq ($(DEBUG_OR_RELEASE), Debug)
	LDFLAGS += -Wl,-debug -Wl,-libpath -Wl,"$(SDK_LOCATION)libraries/ARM_4T/Debug/Default"
	SIM_LIBS += "$(SDK_LOCATION)libraries/Simulator/Debug/PalmOS.lib"
else
	LDFLAGS += -Wl,-libpath -Wl,"$(SDK_LOCATION)libraries/ARM_4T/Release/Default"
	SIM_LIBS += "$(SDK_LOCATION)libraries/Simulator/Release/PalmOS.lib" 
endif

LDOBJS = PalmOS.a SystemGlue.a FloatMgr.sa

ifeq ($(TARGET_PLATFORM), Device)
	NATIVE_OBJ_DIR := $(OBJ_DIR)
	RSC_OBJ_DIR := $(OBJ_DIR)
else
	NATIVE_OBJ_DIR := $(SIM_OBJ_DIR)
	RSC_OBJ_DIR := $(SIM_OBJ_DIR)
endif

# Librarian settings
AR="$(TOOLS_DIR)palib"
SIM_AR=ar

ARFLAGS= -c -a
SIM_ARFLAGS=-rc

LINKER_OUTPUT := $(OBJ_DIR)/$(ESCAPED_ARTIFACT_NAME).axf
SIM_LINKER_OUTPUT := $(SIM_OBJ_DIR)/$(ESCAPED_ARTIFACT_NAME).dll

TARGET	:= $(OBJ_DIR)/$(PRC_NAME)
SIM_TARGET	:= $(SIM_OBJ_DIR)/$(PRC_NAME)

STATIC_LIB_TARGET := $(OBJ_DIR)/$(LIB_NAME)
SIM_STATIC_LIB_TARGET	:= $(SIM_OBJ_DIR)/$(SIM_LIB_NAME)

###############################################################################
# Resource flags (PalmRC)

RFLAGS +=

# Default text encoding is Latin
ifeq ($(PRC_TEXT_ENCODING), )
	PRC_TEXT_ENCODING = LATIN
endif

ifeq ($(PRC_TEXT_ENCODING), LATIN)
	RFLAGS += -target 4.0
else
ifeq ($(PRC_TEXT_ENCODING), JAPANESE)
	RFLAGS += -target 4.0J
else
ifeq ($(PRC_TEXT_ENCODING), SIMPLIFIED_CHINESE)
	RFLAGS += -target 4.0CS
endif
endif
endif

ifeq ($(PRC_NO_LOCALE_CHECK), TRUE)
	RFLAGS += -noLocaleCheck
endif

ifeq ($(PRC_STRICT_LOCALE), TRUE)
	RFLAGS += -strictLocale
endif

ifeq ($(PRC_STRICT_ENCODING), TRUE)
	RFLAGS += -strictTextEncoding
endif

ifdef PRC_OVERLAY_FILTER
	RFLAGS += -overlayFilter $(PRC_OVERLAY_FILTER)
endif

ifeq ($(PRC_NO_WARN_SIZE), TRUE)
	RFLAGS += -noWarnSize
endif

ifeq ($(PRC_QUIET), TRUE)
	RFLAGS += -quiet
endif

ifeq ($(PRCMERGE_QUIET), TRUE)
	PRCFLAGS += -quiet
endif

###############################################################################

# function for converting sources to object file names in one of output directories
define SOURCE_LIST_TO_OBJS
	$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(foreach file, $(SOURCES), \
	$(basename $(notdir $(file))))))
endef

# function for converting sources to dependency file names in one of output directories
define SOURCE_LIST_TO_DEPS
	$(addprefix $(OBJ_DIR)/, $(addsuffix .d, $(foreach file, $(SOURCES), \
	$(basename $(notdir $(file))))))
endef

# function for converting sources to object file names in $(SIM_OBJ_DIR)
define SOURCE_LIST_TO_SIM_OBJS
	$(addprefix $(SIM_OBJ_DIR)/, $(addsuffix .o, $(foreach file, $(SOURCES), \
	$(basename $(notdir $(file))))))
endef

# function for converting sources to dependency file names in $(SIM_OBJ_DIR)
define SOURCE_LIST_TO_SIM_DEPS
	$(addprefix $(SIM_OBJ_DIR)/, $(addsuffix .d, $(foreach file, $(SOURCES), \
	$(basename $(notdir $(file))))))
endef

# function for converting resources into resource object files
define RESOURCE_LIST_TO_OBJS
	$(addprefix $(RSC_OBJ_DIR)/, $(addsuffix .trc, $(foreach file, $(RESOURCES), \
	$(basename $(file)))))
endef

# function for converting resources into resource dependency files
define RESOURCE_LIST_TO_DEPS
	$(addprefix $(OBJ_DIR)/, $(addsuffix .d, $(foreach file, $(RESOURCES), \
	$(basename $(file)))))
endef


OBJS = $(SOURCE_LIST_TO_OBJS)
SIM_OBJS = $(SOURCE_LIST_TO_SIM_OBJS)
SOURCE_PATHS += $(sort $(foreach file, $(SOURCES), $(dir $(file))))
RESOURCE_OBJS = $(RESOURCE_LIST_TO_OBJS)
RESOURCE_PATHS += $(sort $(foreach file, $(RESOURCES), $(dir $(file))))
SLD_BASENAME := $(addsuffix _Startup, $(basename $(notdir $(SLD_FILE))))
SLD_OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SLD_BASENAME)))
SLD_SIM_OBJ := $(addprefix $(SIM_OBJ_DIR)/, $(addsuffix .o, $(SLD_BASENAME)))

DEPS = $(RESOURCE_LIST_TO_DEPS)
ifeq ($(TARGET_PLATFORM), Device)
	DEPS += $(SOURCE_LIST_TO_DEPS)
else
	DEPS += $(SOURCE_LIST_TO_SIM_DEPS)
endif


VPATH :=
VPATH += $(addprefix :, $(subst  ,:, $(filter-out $($(subst, :, ,$(VPATH))), $(SOURCE_PATHS) $(RESOURCE_PATHS) )))

SYSTEM_INCLUDES = $(SYSTEM_INCLUDE_PATHS) -I "$(SDK_LOCATION)headers" -I "$(SDK_LOCATION)headers/posix"
INCLUDES = $(LOCAL_INCLUDE_PATHS) $(foreach path, $(SOURCE_PATHS), $(addprefix -I, $(path))) $(SYSTEM_INCLUDES) $(PRE_INCLUDE_PATHS)
CPP_INCLUDES = -I "$(TOOLS_DIR)include/stlport"

SIM_SYSTEM_INCLUDES = $(SYSTEM_SIM_INCLUDE_PATHS) -I "$(SDK_LOCATION)headers" -I "$(SDK_LOCATION)headers/posix"
SIM_INCLUDES = $(LOCAL_SIM_INCLUDE_PATHS) $(foreach path, $(SOURCE_PATHS), $(addprefix -I, $(path))) $(SIM_SYSTEM_INCLUDES) $(SIM_PRE_INCLUDE_PATHS)
SIM_CPP_INCLUDES = -I "$(TOOLS_DIR)include/stlport"

# Now add additional settings specified by user
LDFLAGS += $(ADDITIONAL_LINK_LIBRARIES)
LDFLAGS += $(foreach libpath, $(ADDITIONAL_LINK_LIBPATH), $(addprefix -libpath , $(libpath)))
LDFLAGS += $(foreach option, $(ADDITIONAL_LINKER_FLAGS), $(addprefix -Wl$(comma), $(option)))
CFLAGS += $(foreach define, $(DEFINES), $(addprefix -D, $(define)))
CFLAGS += $(foreach define, $(UNDEFINES), $(addprefix -U, $(define)))
CFLAGS += $(ADDITIONAL_COMPILER_FLAGS)

SIM_LDFLAGS += $(ADDITIONAL_SIM_LINK_LIBRARIES)
SIM_LDFLAGS += $(ADDITIONAL_SIM_LINKER_FLAGS)
SIM_LDFLAGS += $(foreach libpath, $(ADDITIONAL_SIM_LINK_LIBPATH), $(addprefix -L, $(libpath)))
SIM_CFLAGS += $(foreach define, $(SIM_DEFINES), $(addprefix -D, $(define)))
SIM_CFLAGS += $(foreach define, $(SIM_UNDEFINES), $(addprefix -U, $(define)))
SIM_CFLAGS += $(ADDITIONAL_SIM_COMPILER_FLAGS)

# Specifyc additional archival flags (for static libraries)
ARFLAGS += $(ADDITIONAL_AR_FLAGS)
SIM_ARFLAGS += $(ADDITIONAL_SIM_AR_FLAGS)

RFLAGS += $(ADDITIONAL_PRC_FLAGS)
PRCFLAGS += $(ADDITIONAL_PRCMERGE_FLAGS)

###############################################################################
# Flags for PRC creation

PRCFLAGS += -dbFlagExtendedDB -dbFlagExecutable

PRCFLAGS += -dbVersion $(DATABASE_VERSION) $(DATABASE_PROTECT) $(DATABASE_HIDDEN) $(DATABASE_BACKUP) $(DATABASE_RESET) $(DATABASE_BUNDLE)


###############################################################################
# Flags for pslib

PSLIB_DEV_FLAGS =
PSLIB_SIM_FLAGS = -outSimgcc
ifeq ($(PROJECT_TYPE), slib)
	PSLIB_DEV_FLAGS += -outEntryNums $(OBJ_DIR)/$(ESCAPED_ARTIFACT_NAME)_Client.h -outObjStubs $(OBJ_DIR)/$(ESCAPED_ARTIFACT_NAME)_Client.sa 
	PSLIB_SIM_FLAGS += -outEntryNums $(SIM_OBJ_DIR)/$(ESCAPED_ARTIFACT_NAME)_Client.h -outSimStubs $(SIM_OBJ_DIR)/$(ESCAPED_ARTIFACT_NAME)_Client.slib
endif



###############################################################################
# Project make target determination

ifeq ($(TARGET_PLATFORM), Device)

ifeq ($(PROJECT_TYPE), )
	PROJECT_TARGET = device
endif

ifeq ($(PROJECT_TYPE), appl)
	PROJECT_TARGET = device
endif
ifeq ($(PROJECT_TYPE), slib)
	PROJECT_TARGET = device_shared_lib
endif
ifeq ($(PROJECT_TYPE), lib)
	PROJECT_TARGET = device_static_lib
endif

else

ifeq ($(PROJECT_TYPE), )
	PROJECT_TARGET = simulator
endif

ifeq ($(PROJECT_TYPE), appl)
	PROJECT_TARGET = simulator
endif
ifeq ($(PROJECT_TYPE), slib)
	PROJECT_TARGET = simulator_shared_lib
endif
ifeq ($(PROJECT_TYPE), lib)
	PROJECT_TARGET = simulator_static_lib
endif

endif

###############################################################################
# Dependency includes

-include $(DEPS)


###############################################################################
#	Eclipse requires an all target to get the work done

all:  $(PROJECT_TARGET)

#	This rule is only valid for projects created as application projects.
#	Don't invoke this make target directly; instead change the value of
#	TARGET_PLATFORM to Device in the main makefile
device: $(OBJ_DIR) $(TARGET)

#	This rule is only valid for projects created as application projects.
#	Don't invoke this make target directly; instead change the value of
#	TARGET_PLATFORM to Simulator in the main makefile
simulator: $(SIM_OBJ_DIR) $(SIM_TARGET)

#	This rule is only valid for projects created as shared library projects.
#	Don't invoke this make target directly; instead change the value of
#	TARGET_PLATFORM to Device in the main makefile
device_shared_lib: $(OBJ_DIR) $(TARGET)

#	This rule is only valid for projects created as shared library projects.
#	Don't invoke this make target directly; instead change the value of
#	TARGET_PLATFORM to Simulator in the main makefile
simulator_shared_lib: $(SIM_OBJ_DIR) $(SIM_TARGET)

#	This rule is only valid for projects created as static library projects.
#	Don't invoke this make target directly; instead change the value of
#	TARGET_PLATFORM to Device in the main makefile
device_static_lib: $(OBJ_DIR) $(STATIC_LIB_TARGET)

#	This rule is only valid for projects created as static library projects.
#	Don't invoke this make target directly; instead change the value of
#	TARGET_PLATFORM to Simulator in the main makefile
simulator_static_lib: $(SIM_OBJ_DIR) $(SIM_STATIC_LIB_TARGET)


# rule to create the object file directories if needed
$(OBJ_DIR):
	@[ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR) > /dev/null 2>&1

$(SIM_OBJ_DIR):
	@[ -d $(SIM_OBJ_DIR) ] || mkdir $(SIM_OBJ_DIR) > /dev/null 2>&1
	
# main C/C++ sources
$(OBJ_DIR)/%.o : %.c 
	@echo 
	$(CC) -c $< $(INCLUDES) $(CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $@ : ' ' > $(@:%.o=%.d) && \
	$(CC) $< $(DEPFLAGS) $(INCLUDES) $(CFLAGS) | /usr/bin/sed 's/^.*: //' | /usr/bin/sed 's/ /\\\ /g' | /usr/bin/tr '\r\n' ' ' >> $(@:%.o=%.d) && \
	echo -e \\n\\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(OBJ_DIR)/%.o : %.cpp 
	@echo 
	$(CC) -c $< $(INCLUDES) $(CPP_INCLUDES) $(CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $@ : ' ' > $(@:%.o=%.d) && \
	$(CC) $< $(DEPFLAGS) $(INCLUDES) $(CPP_INCLUDES) $(CFLAGS) | /usr/bin/sed 's/^.*: //' | /usr/bin/sed 's/ /\\\ /g' | /usr/bin/tr '\r\n' ' ' >> $(@:%.o=%.d) && \
	echo -e \\n\\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(OBJ_DIR)/%.o : %.cp 
	@echo 
	$(CC) -c $< $(INCLUDES) $(CPP_INCLUDES) $(CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $@ : ' ' > $(@:%.o=%.d) && \
	$(CC) $< $(DEPFLAGS) $(INCLUDES) $(CPP_INCLUDES) $(CFLAGS) | /usr/bin/sed 's/^.*: //' | /usr/bin/sed 's/ /\\\ /g' | /usr/bin/tr '\r\n' ' ' >> $(@:%.o=%.d) && \
	echo -e \\n\\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(OBJ_DIR)/%.o : %.cc 
	@echo 
	$(CC) -c $< $(INCLUDES) $(CPP_INCLUDES) $(CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $@ : ' ' > $(@:%.o=%.d) && \
	$(CC) $< $(DEPFLAGS) $(INCLUDES) $(CPP_INCLUDES) $(CFLAGS) | /usr/bin/sed 's/^.*: //' | /usr/bin/sed 's/ /\\\ /g' | /usr/bin/tr '\r\n' ' ' >> $(@:%.o=%.d) && \
	echo -e \\n\\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(OBJ_DIR)/%.o : %.C 
	@echo 
	$(CC) -c $< $(INCLUDES) $(CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $@ : ' ' > $(@:%.o=%.d) && \
	$(CC) $< $(DEPFLAGS) $(INCLUDES) $(CFLAGS) | /usr/bin/sed 's/^.*: //' | /usr/bin/sed 's/ /\\\ /g' | /usr/bin/tr '\r\n' ' ' >> $(@:%.o=%.d) && \
	echo -e \\n\\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(OBJ_DIR)/%.o : %.CC 
	@echo 
	$(CC) -c $< $(INCLUDES) $(CPP_INCLUDES) $(CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $@ : ' ' > $(@:%.o=%.d) && \
	$(CC) $< $(DEPFLAGS) $(INCLUDES) $(CPP_INCLUDES) $(CFLAGS) | /usr/bin/sed 's/^.*: //' | /usr/bin/sed 's/ /\\\ /g' | /usr/bin/tr '\r\n' ' ' >> $(@:%.o=%.d) && \
	echo -e \\n\\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(OBJ_DIR)/%.o : %.CPP 
	@echo 
	$(CC) -c $< $(INCLUDES) $(CPP_INCLUDES) $(CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $@ : ' ' > $(@:%.o=%.d) && \
	$(CC) $< $(DEPFLAGS) $(INCLUDES) $(CPP_INCLUDES) $(CFLAGS) | /usr/bin/sed 's/^.*: //' | /usr/bin/sed 's/ /\\\ /g' | /usr/bin/tr '\r\n' ' ' >> $(@:%.o=%.d) && \
	echo -e \\n\\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	

# Simulator C/C++ sources
$(SIM_OBJ_DIR)/%.o : %.c 
	@echo 
	$(SIM_CC) -c $< $(SIM_INCLUDES) -std=c99 $(SIM_CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	$(SIM_CC) -c $< $(SIM_DEPFLAGS) $(SIM_INCLUDES) -std=c99 $(SIM_CFLAGS) >> $(@:%.o=%.d) && \
	echo -e \\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(SIM_OBJ_DIR)/%.o : %.cpp 
	@echo 
	$(SIM_CC) -c $< $(SIM_INCLUDES) $(SIM_CPP_INCLUDES) -frtti $(SIM_CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	$(SIM_CC) -c $< $(SIM_DEPFLAGS) $(SIM_INCLUDES) $(SIM_CPP_INCLUDES) -frtti $(SIM_CFLAGS) >> $(@:%.o=%.d) && \
	echo -e \\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(SIM_OBJ_DIR)/%.o : %.cp 
	@echo 
	$(SIM_CC) -c $< $(SIM_INCLUDES) $(SIM_CPP_INCLUDES) -frtti $(SIM_CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	$(SIM_CC) -c $< $(SIM_DEPFLAGS) $(SIM_INCLUDES) $(SIM_CPP_INCLUDES) -frtti $(SIM_CFLAGS) >> $(@:%.o=%.d) && \
	echo -e \\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(SIM_OBJ_DIR)/%.o : %.cc 
	@echo 
	$(SIM_CC) -c $< $(SIM_INCLUDES) $(SIM_CPP_INCLUDES) -frtti $(SIM_CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	$(SIM_CC) -c $< $(SIM_DEPFLAGS) $(SIM_INCLUDES) $(SIM_CPP_INCLUDES) -frtti $(SIM_CFLAGS) >> $(@:%.o=%.d) && \
	echo -e \\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(SIM_OBJ_DIR)/%.o : %.C 
	@echo 
	$(SIM_CC) -c $< $(SIM_INCLUDES)  -std=c99 $(SIM_CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	$(SIM_CC) -c $< $(SIM_DEPFLAGS) $(SIM_INCLUDES) -std=c99 $(SIM_CFLAGS) >> $(@:%.o=%.d) && \
	echo -e \\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(SIM_OBJ_DIR)/%.o : %.CC 
	@echo 
	$(SIM_CC) -c $< $(SIM_INCLUDES) $(SIM_CPP_INCLUDES) -frtti $(SIM_CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	$(SIM_CC) -c $< $(SIM_DEPFLAGS) $(SIM_INCLUDES) $(SIM_CPP_INCLUDES) -frtti $(SIM_CFLAGS) >> $(@:%.o=%.d) && \
	echo -e \\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)
	
$(SIM_OBJ_DIR)/%.o : %.CPP 
	@echo 
	$(SIM_CC) -c $< $(SIM_INCLUDES) $(SIM_CPP_INCLUDES) -frtti $(SIM_CFLAGS) -o $@
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	$(SIM_CC) -c $< $(SIM_DEPFLAGS) $(SIM_INCLUDES) $(SIM_CPP_INCLUDES) -frtti $(SIM_CFLAGS) >> $(@:%.o=%.d) && \
	echo -e \\n$(@:%.o=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.o=%.d)


# XRD source processing
$(RSC_OBJ_DIR)/%.trc : %.xrd 
	@echo 
	"$(TOOLS_DIR)PalmRC" -p $(TARGET_FORMAT) $(RFLAGS) -locale $(LOCALE) $< -o $@
	echo -n $(@:%.trc=%.d) $@ : ' ' > $(@:%.trc=%.d) && \
	"$(TOOLS_DIR)PalmRC" -quiet -p $(TARGET_FORMAT) -makedeps $(@:%.trc=%.d).tmp $(RFLAGS) -locale $(LOCALE) $< && \
	/usr/bin/cygpath -m -f $(@:%.trc=%.d).tmp | /usr/bin/sed 's/ /\\\ /g' | /usr/bin/tr '\r\n' ' ' >> $(@:%.trc=%.d) && \
	rm -rf $(@:%.trc=%.d).tmp && \
	echo -e \\n\\n$(@:%.trc=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.trc=%.d)
	
$(RSC_OBJ_DIR)/%.trc : %.XRD 
	@echo 
	"$(TOOLS_DIR)PalmRC" -p $(TARGET_FORMAT) $(RFLAGS) -locale $(LOCALE) $< -o $@
	echo -n $(@:%.trc=%.d) $@ : ' ' > $(@:%.trc=%.d) && \
	"$(TOOLS_DIR)PalmRC" -quiet -p $(TARGET_FORMAT) -makedeps $(@:%.trc=%.d).tmp $(RFLAGS) -locale $(LOCALE) $< && \
	/usr/bin/cygpath -m -f $(@:%.trc=%.d).tmp | /usr/bin/sed 's/ /\\\ /g' | /usr/bin/tr '\r\n' ' ' >> $(@:%.trc=%.d) && \
	rm -rf $(@:%.trc=%.d).tmp && \
	echo -e \\n\\n$(@:%.trc=%.d) $@ : makefile makefile-engine.mk auto-generated.mk >> $(@:%.trc=%.d)
	


# Definition file source processing
$(SLD_OBJ) : makefile makefile-engine.mk auto-generated.mk 
	@echo 
	@echo "...Processing definition file for Device..."
	"$(TOOLS_DIR)pslib" -inDef "$(SLD_FILE)" $(PSLIB_DEV_FLAGS) -outObjStartup $@ -type '$(DB_TYPE)' -creator '$(CREATOR_ID)' -execName $(ESCAPED_ARTIFACT_NAME)
	
$(SLD_SIM_OBJ) : makefile makefile-engine.mk auto-generated.mk 
	@echo 
	@echo "...Processing definition file for Simulator..."
	"$(TOOLS_DIR)pslib" -inDef "$(SLD_FILE)" $(PSLIB_SIM_FLAGS) -outSimStartup $@ -outSimRsrc $(SIM_OBJ_DIR)/acod0000.bin  -outSimDefs $(SIM_OBJ_DIR)/gcc_link.def -type '$(DB_TYPE)' -creator '$(CREATOR_ID)' -execName $(ESCAPED_ARTIFACT_NAME)

# Linking step
$(LINKER_OUTPUT) : $(OBJS)
	@echo 
	@echo "...Linking for Device Target..."
	$(LD) -o $(LINKER_OUTPUT) $(OBJS) $(SLD_OBJ) $(LDOBJS) $(LDFLAGS)

$(SIM_LINKER_OUTPUT) : $(SIM_OBJS)
	@echo 
	@echo "...Linking for Simulator Target..."
	$(SIM_LD) --entry 0 -o $(SIM_LINKER_OUTPUT) $(SIM_OBJS) $(SLD_SIM_OBJ) $(SIM_LDFLAGS) $(SIM_LIBS)


# Final target creation
$(TARGET): $(SLD_OBJ) $(LINKER_OUTPUT) $(RESOURCE_OBJS) 
	@echo 
	@echo "...Creating PRC for Device: $(TARGET)..."
	"$(TOOLS_DIR)pelf2bin" -format 6 -directory $(OBJ_DIR) -code acod -data adat -rsrc 0x0000 $(LINKER_OUTPUT)
	@echo
	"$(TOOLS_DIR)PRCMerge" -dbType '$(DB_TYPE)' -dbCreator '$(CREATOR_ID)' -dbName $(DATABASE_NAME) $(PRCFLAGS) -o $(TARGET) $(RESOURCE_OBJS) $(OBJ_DIR)/adat0000.bin $(OBJ_DIR)/acod0000.bin
	@echo ...Done with Device build...
	@echo 

$(SIM_TARGET): $(SLD_SIM_OBJ) $(SIM_LINKER_OUTPUT) $(RESOURCE_OBJS)
	@echo 
	@echo "...Creating PRC for Simulator: $(SIM_TARGET)..."
	cp -f "$(SDK_LOCATION)misctools/Resources/amdd_fake" $(SIM_OBJ_DIR)/adat0000.bin
	@echo
	"$(TOOLS_DIR)PRCMerge" -dbType '$(DB_TYPE)' -dbCreator '$(CREATOR_ID)' -dbName $(DATABASE_NAME) $(PRCFLAGS) -o $(SIM_TARGET) $(RESOURCE_OBJS) $(SIM_OBJ_DIR)/adat0000.bin $(SIM_OBJ_DIR)/acod0000.bin
	@echo ...Done with Simulator build...
	@echo 

$(STATIC_LIB_TARGET) : $(OBJS)
	@echo 
	@echo "...Linking Static Library for Device Target..."
	$(AR) $(ARFLAGS) $(STATIC_LIB_TARGET) $(OBJS) 

$(SIM_STATIC_LIB_TARGET) : $(SIM_OBJS)
	@echo 
	@echo "...Linking Static Library for Simulator Target..."
	$(SIM_AR) $(SIM_ARFLAGS) $(SIM_STATIC_LIB_TARGET) $(SIM_OBJS) 


FORCE:

# Eclipse requires a clean command
clean :: FORCE
	-rm -rf $(NATIVE_OBJ_DIR)/*

