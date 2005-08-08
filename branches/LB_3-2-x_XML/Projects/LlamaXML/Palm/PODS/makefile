## --------------------------------------------------------------------------
#  Palm OS Generic Protein Makefile for Palm OS Developer Suite v1.1.0
#
# Last edit: 10/14/04
#
# Fill in this file to specify your project and the source that you want
# to build, and the settings involved in the build. The makefile-engine.mk
# will then do the hard work of the makefile and dependency handling.
#
# After starting a new project, please remember the following steps...
#	1. Add all sources and resources in SOURCES and RESOURCES
#	2. Review the other settings as needed.
#
## --------------------------------------------------------------------------

SHELL = /bin/sh

## --------------------------------------------------------------------------
# Set up the artifact name, which is the root name of your project's output
# without the extension.
## --------------------------------------------------------------------------

ARTIFACT_NAME =LlamaXML

EMPTY =
SPACE =$(EMPTY) $(EMPTY)
ESCAPED_ARTIFACT_NAME = $(subst $(SPACE),\ ,$(ARTIFACT_NAME))

## --------------------------------------------------------------------------
# Sources and Resources and Definition files
# List all the sources (.c/.cpp), resources (.xrd), and definition file (.sld)
# in your project.  Use project relative path names with forward slashes 
# (src/code.cpp).  Please do not use spaces in directory or file names.
#
# A note about XRD resource files:  If you have existing .rsrc or .rcp files, 
# refer to the documentation for the GenerateXRD tool to convert them into 
# XRD files for use with all Palm OS SDKs.
## --------------------------------------------------------------------------

# TODO: Update all sources and resources

SOURCES = $(wildcard ../../Common/*.cpp) $(wildcard ../Source/*.cpp)
RESOURCES = 
SLD_FILE = 

## --------------------------------------------------------------------------
# Review Database information
# Register Creator ID at: http://www.palmos.com/dev/creatorid 
## --------------------------------------------------------------------------

CREATOR_ID = STRT
DB_TYPE = appl

# to enable reset on install, use -r
DATABASE_RESET = 
# to enable backup, use -b
DATABASE_BACKUP = -b
# to make the app hidden, use -h
DATABASE_HIDDEN = 
# to prevent copy, use -p
DATABASE_PROTECT = 
# to bundle the application with its databases on beaming, use -dbFlagBundle
DATABASE_BUNDLE = 

DATABASE_VERSION = 1

LOCALE =  enUS

ESCAPED_DATABASE_NAME = $(subst $(SPACE),\ ,LlamaXML)
DATABASE_NAME =$(ESCAPED_DATABASE_NAME) 

## --------------------------------------------------------------------------
# Build Settings
# Review the following for your needs.
# The default settings build with debug information and no optimization and
# a target platform of an actual device.
## --------------------------------------------------------------------------

#
# Set Debug or Release configuration for this project via the project 
# properties (right-click on the project and select Properties).
#
DEBUG_OR_RELEASE=Release

#
# Set the target platform for the build; either Device or Simulator
# Use Device as a target for emulator builds.
#
TARGET_PLATFORM=Simulator

# Specify the level of optimization that you want:
# NONE, SOME, FULL, INTRAPROCEDURAL, INTERPROCEDURAL, INLINING.
# Leave blank to select FULL for release builds, and NONE for
# debug builds.
# INTRAPROCEDURAL, INTERPROCEDURAL, INLINING are applicable to 
# device builds only; simulator builds will translate those levels
# to FULL.
OPTIMIZE_LEVEL = 

# Specify warning level for device target compilation
# NONE = suppress all warnings
# SOME = enable some warnings
# ALL = enable all warnings
# <undefined> = default warnings
WARNING_LEVEL = SOME

# Specify warning level for simulator target compilation
# NONE = suppress all warnings
# ALL = enable all warnings
# <undefined> = default warnings
SIM_WARNING_LEVEL = 

# Specify if warnings are treated as errors 
# TRUE, FALSE
# for device compilations, set WARNING_AS_ERROR
# for simulator compilations, set SIM_WARNING_AS_ERROR
WARNING_AS_ERROR = 
SIM_WARNING_AS_ERROR = 

# Specify verbose output for device build
# TRUE, FALSE
# for device compilations, set VERBOSE
# for simulator compilations, set SIM_VERBOSE
VERBOSE = 
SIM_VERBOSE = 
ARM_LOGO = FALSE

# Specify exception handling support
# TRUE, FALSE
ENABLE_EXCEPTIONS = TRUE

# Specify assertion support
# TRUE, FALSE
ENABLE_ASSERTIONS = TRUE

# Additional paths to look for #include "header"
# (Source file directories are automatically included)
# Please note that both local and system include paths should 
# either use "cygwin" syntax or if in Windows syntax the should
# be quoted; i.e. ../MyLibrary/headers or "C:\Source\MyLibrary\headers".
# Additionally, you must explicly specify the "-I" prior to each
# path included in this variable.  Spaces are used to separate
# each path from each other.
LOCAL_INCLUDE_PATHS = 
LOCAL_SIM_INCLUDE_PATHS = 

# Additional paths to look for #include <header>
# (Palm OS SDK directories are automatically included)
# Additionally, you must explicly specify the "-I" prior to each
# path included in this variable.  Spaces are used to separate
# each path from each other.
SYSTEM_INCLUDE_PATHS = 
SYSTEM_SIM_INCLUDE_PATHS = 
PRE_INCLUDE_PATHS = 
SIM_PRE_INCLUDE_PATHS = 

# Specify any needed preprocessor symbols.
# If you set DEFINES to "ASSERTLEVEL=1", the compiler will see "-DASSERTLEVEL=1"
# (separate individual entries with spaces)
DEFINES = 
UNDEFINES = 
SIM_DEFINES = 
SIM_UNDEFINES = 

# Specify additional compiler flags for all files
ADDITIONAL_COMPILER_FLAGS = 
ADDITIONAL_SIM_COMPILER_FLAGS = 

# Specify additional linker flags
ADDITIONAL_LINKER_FLAGS = 
ADDITIONAL_SIM_LINKER_FLAGS = 
ADDITIONAL_LINK_LIBRARIES = 
ADDITIONAL_SIM_LINK_LIBRARIES = 
ADDITIONAL_LINK_LIBPATH = 
ADDITIONAL_SIM_LINK_LIBPATH = 

# Specify additional archival flags (for static libraries)
ADDITIONAL_AR_FLAGS = 
ADDITIONAL_SIM_AR_FLAGS = 

# Handle palink specific flags
ADDITIONAL_PALINK_FLAGS =    

## --------------------------------------------------------------------------
# Resource Settings
## --------------------------------------------------------------------------

# Specify the target text encoding
# LATIN, JAPANESE, SIMPLIFIED_CHINESE
PRC_TEXT_ENCODING = LATIN

# Specify resource locale checking
# TRUE, FALSE
PRC_NO_LOCALE_CHECK = 

# Specify strict resource locale checking
# TRUE, FALSE
PRC_STRICT_LOCALE = 

# Specify if text encoding should be checked
# TRUE, FALSE
PRC_STRICT_ENCODING = TRUE

# Specify overlay filter value
# BASE, OVERLAY, FULL, <default>
PRC_OVERLAY_FILTER = 

# Specify no warning on resource size
# TRUE, FALSE
PRC_NO_WARN_SIZE = 

# Specify quiet output or not (opposite of verbose)
# Use PRC_QUIET for PalmRC invocations
# Use PRCMERGE_QUIET for PRCMerge invocations
# TRUE, FALSE
PRC_QUIET = TRUE
PRCMERGE_QUIET = TRUE

# Additional flags for Resource compiler and linker
ADDITIONAL_PRC_FLAGS =  
ADDITIONAL_PRCMERGE_FLAGS =    

## --------------------------------------------------------------------------
# Output Settings
## --------------------------------------------------------------------------

# Modify if you want object and output files to be in different locations
# (However, please note that all four object directories must be different
# for the makefile engine to properly operate.)

DEBUG_DEVICE_OUTPUT_DIR := DebugDevice
RELEASE_DEVICE_OUTPUT_DIR := ReleaseDevice
DEBUG_SIMULATOR_OUTPUT_DIR := DebugSim
RELEASE_SIMULATOR_OUTPUT_DIR := ReleaseSim

## --------------------------------------------------------------------------
# Makefiles
## --------------------------------------------------------------------------

## conditionally include an auto-generated.mk for dynamic definitions
-include auto-generated.mk

## include the makefile engine
include makefile-engine.mk

## conditionally include your own custom.mk for your specific overrides or definitions
## this is useful if you don't want to use the auto-generated.mk values
## for some reason
-include custom.mk
