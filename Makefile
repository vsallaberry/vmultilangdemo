#
# Copyright (C) 2018 Vincent Sallaberry
# vmultilangdemo <https://github.com/vsallaberry/vmultilangdemo>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
############################################################################################
#
# vmultilangdemo: a multiple language/meta-language demo program (c/c++/java/objc/bison/flex).
#   need bison 3, gcj, gcc, g++.
#
# Generic Makefile for GNU-like or BSD-like make (paths with spaces not supported).
#   from vlib,vsensorsdemo,libvsensors Copyright (C) 2017-2018 Vincent Sallaberry
############################################################################################

# First, 'all' rule calling default_rule to allow user adding his own dependency
# rules in specific part below.
all: default_rule

#############################################################################################
# PROJECT SPECIFIC PART
#############################################################################################

# SRCDIR: Folder where sources are. Use '.' for current directory. MUST NEVER BE EMPTY !!
# Folders which contains a Makefile are ignored, you have to add them in SUBDIRS and update SUBLIBS.
SRCDIR 		= .

# SUBDIRS, put empty if there is no need to run make on sub directories.
LIB_VLIBDIR	= ext/vlib
#SUBDIRS 	= $(LIB_VLIBDIR)
SUBDIRS		=
# SUBLIBS: libraries built from subdirs, needed for binary dependency. Put empty if none.
LIB_VLIB	= $(LIB_VLIBDIR)/libvlib.a
#SUBLIBS		= $(LIB_VLIB)
SUBLIBS		=

# INCDIRS: Folder where public includes are. It can be SRCDIR or even empty if
# headers are only in SRCDIR. Use '.' for current directory.
#INCDIRS 	= $(LIB_VLIBDIR)/include
INCDIRS 	=

# INSTALLINCDIRS: the list of folders with includes to be installed in $(PREFIX)/include
INSTALLINCDIRS	=

# Where targets are created (OBJs, BINs, ...). Eg: '.' or 'build'. ONLY 'SRCDIR' is supported!
BUILDDIR	= $(SRCDIR)

# Name of the Package (DISTNAME, BIN and LIB depends on it)
NAME		= vmultilangdemo

# Binary name and library name (prefix with '$(BUILDDIR)/' to put it in build folder).
# Fill LIB and set BIN,JAR empty to create a library, or clear LIB,JAR and set BIN to create a binary.
BIN		= $(NAME)
LIB		=
JAR		=

# DISTDIR: where the dist packages zip/tar.xz are saved
DISTDIR		= ../../dist

# PREFIX: where the application is to be installed
PREFIX		= /usr/local

# Project specific Flags (system specific flags are handled further)
# Choice between <flag>_RELEASE/_DEBUG is done according to BUILDINC
WARN_RELEASE	= -Wall -W -pedantic # -Wno-ignored-attributes -Wno-attributes
ARCH_RELEASE	= -march=native # -arch i386 -arch x86_64
OPTI_RELEASE	= -O3 -pipe
INCS_RELEASE	=
LIBS_RELEASE	= $(SUBLIBS)
MACROS_RELEASE	=
WARN_DEBUG	= $(WARN_RELEASE) # -Werror
ARCH_DEBUG	= $(ARCH_RELEASE)
OPTI_DEBUG	= -O0 -ggdb3
INCS_DEBUG	= $(INCS_RELEASE)
LIBS_DEBUG	= $(LIBS_RELEASE)
MACROS_DEBUG	= -D_DEBUG -D_TEST
# FLAGS_<lang> is global for one language (lang: C,CXX,OBJC,GCJ).
FLAGS_C		= -std=c99 -D_GNU_SOURCE
FLAGS_CXX	= -D_GNU_SOURCE
FLAGS_OBJC	= -std=c99
FLAGS_GCJ	=
# FLAGS_<lang>_<file> is specific to one file (eg: 'FLAGS_CXX_Big.cc=-O0')

# System specific flags (WARN_$(sys),OPTI_$(sys),DEBUG_$(sys),LIBS_$(sys),INCS_$(sys))
# $(sys) is lowcase(`uname`), eg: 'LIBS_darwin=-framework IOKit -framework Foundation'
LIBS_darwin	= -framework IOKit -framework Foundation

############################################################################################
# GENERIC PART - in most cases no need to change anything below until end of file
############################################################################################

AR		= ar
RANLIB		= ranlib
GREP		= grep
WHICH		= which
HEADN1		= head -n1
PRINTF		= printf
AWK		= awk
SED		= sed
GREP		= grep
RM		= rm -f
DATE		= date
TAR		= tar
ZIP		= zip
FIND		= find
PKGCONFIG	= pkg-config
TEST		= test
SORT		= sort
MKDIR		= mkdir
RMDIR		= rmdir
TOUCH		= touch
CP		= CP
MV		= mv
TR		= tr
GIT		= git
DIFF		= diff
UNIQ		= uniq
INSTALL		= install -c -m 0644
INSTALLBIN	= install -c
INSTALLDIR	= install -c -d
NO_STDERR	= 2> /dev/null
NO_STDOUT	= > /dev/null

############################################################################################
# About shell commands execution in this Makefile:
# - On recent gnu make, "!=' is understood.
# - On gnu make 3.81 '!=' is not understood but it does NOT cause syntax error.
# - On {open,free,net}bsd $(shell cmd) is not understood but does NOT cause syntax error.
# - On gnu make 3.82, '!=' causes syntax error, then it is at the moment only case where
#   make-fallback is needed (make-fallback removes lines which cannot be parsed).
# Assuming that, the command to be run is put in a variable (cmd_...),
# then the '!=' is tried, and $(shell ..) will be done only on '!=' failure (?= $(shell ..).
# It is important to use a temporary name, like tmp_CC because CC is set by make at startup.
# Generally, we finish the command by true as some bsd make raise warnings if not.
############################################################################################

# Important to prefix with ., to make VERSIONINC and BUILDINC excluded from include search.
BUILDINC	= ./build.h
VERSIONINC	= ./version.h
SYSDEPDIR	= sysdeps

# SRCINC containing source code is included if APP_INCLUDE_SOURCE is defined in VERSIONINC.
SRCINCNAME	= _src_.c
SRCINCDIR	= $(BUILDDIR)

cmd_SRCINC	= ! $(TEST) -e $(VERSIONINC) \
		  || $(GREP) -Eq '^[[:space:]]*\#[[:space:]]*define APP_INCLUDE_SOURCE([[:space:]]|$$)' \
	                                $(VERSIONINC) $(NO_STDERR) && echo $(SRCINCDIR)/$(SRCINCNAME) || true
tmp_SRCINC	!= $(cmd_SRCINC)
tmp_SRCINC	?= $(shell $(cmd_SRCINC))
SRCINC		:= $(tmp_SRCINC)

# Forbid space between '#' and 'define' so as word number is fixed (easy parsing)
cmd_VERSION	= $(AWK) '/^[ \t]*\#define APP_VERSION[ \s]/ { print $$3; }' $(VERSIONINC) $(NO_STDERR) || true
tmp_VERSION	!= $(cmd_VERSION)
tmp_VERSION	?= $(shell $(cmd_VERSION))
VERSION		:= $(tmp_VERSION)

# Get Debug mode in build.h
cmd_RELEASEMODE	= $(GREP) -Eq '^[[:space:]]*\#[[:space:]]*define BUILD_DEBUG([[:space:]]|$$)' \
				$(BUILDINC) $(NO_STDERR) && echo DEBUG || echo RELEASE
tmp_RELEASEMODE	!= $(cmd_RELEASEMODE)
tmp_RELEASEMODE	?= $(shell $(cmd_RELEASEMODE))
RELEASE_MODE	:= $(tmp_RELEASEMODE)

WARN		= $(WARN_$(RELEASE_MODE))
OPTI		= $(OPTI_$(RELEASE_MODE))
ARCH		= $(ARCH_$(RELEASE_MODE))
INCS		= $(INCS_$(RELEASE_MODE))
LIBS		= $(LIBS_$(RELEASE_MODE))
MACROS		= $(MACROS_$(RELEASE_MODE))
cmd_TOPDIR	= pwd
tmp_TOPDIR	!= $(cmd_TOPDIR)
tmp_TOPDIR	?= $(shell $(cmd_TOPDIR))
TOPDIR		:= $(tmp_TOPDIR)

############################################################################################
# Scan for sources
############################################################################################
# Common find pattern to include files in SRCDIR/sysdeps ONLY if suffixed with system name.
find_AND_SYSDEP	= -and \( \! -path '$(SRCDIR)/$(SYSDEPDIR)/*' -or -path '$(SRCDIR)/$(SYSDEPDIR)/*$(SYSDEP_SUF).*' \)

# Search Meta sources (used to generate sources)
# For yacc/bison and lex/flex:
#   - the basename of meta sources must be always different (have one grammar calc.y and one
#     lexer calc.l is not supported: prefer parse-calc.y and scan-calc.l).
#   - c++ source is generated with .ll and .yy, c source with .l and .y, java with .yyj
#   - yacc generates by default headers for lexer, therefore lexer files depends on parser files.
cmd_YACCSRC	= $(FIND) $(SRCDIR) \( -name '*.y' -or -name '*.yy' \) $(find_AND_SYSDEP)
cmd_LEXSRC	= $(FIND) $(SRCDIR) \( -name '*.l' -or -name '*.ll' \) $(find_AND_SYSDEP)
cmd_YACCJAVA	= $(FIND) $(SRCDIR) -name '*.yyj' $(find_AND_SYSDEP)
# METASRC variable, filled from the 'find' command (cmd_{YACC,LEX,..}SRC) defined above.
tmp_YACCSRC	!= $(cmd_YACCSRC)
tmp_YACCSRC	?= $(shell $(cmd_YACCSRC))
YACCSRC		:= $(tmp_YACCSRC)
tmp_LEXSRC	!= $(cmd_LEXSRC)
tmp_LEXSRC	?= $(shell $(cmd_LEXSRC))
LEXSRC		:= $(tmp_LEXSRC)
tmp_YACCJAVA	!= $(cmd_YACCJAVA)
tmp_YACCJAVA	?= $(shell $(cmd_YACCJAVA))
YACCJAVA	:= $(tmp_YACCJAVA)
METASRC		:= $(YACCSRC) $(LEXSRC) $(YACCJAVA)
# Transform meta sources into sources and objects
tmp_YACCGENSRC1	= $(YACCSRC:.y=.c)
YACCGENSRC	:= $(tmp_YACCGENSRC1:.yy=.cc)
tmp_LEXGENSRC1	= $(LEXSRC:.l=.c)
LEXGENSRC	:= $(tmp_LEXGENSRC1:.ll=.cc)
YACCGENJAVA	:= $(YACCJAVA:.yyj=.java)
tmp_YACCOBJ1	= $(YACCGENSRC:.c=.o)
YACCOBJ		:= $(tmp_YACCOBJ1:.cc=.o)
tmp_LEXOBJ1	= $(LEXGENSRC:.c=.o)
LEXOBJ		:= $(tmp_LEXOBJ1:.cc=.o)
YACCCLASSES	:= $(YACCGENJAVA:.java=.class)
tmp_YACCINC1	= $(YACCSRC:.y=.h)
YACCINC		:= $(tmp_YACCINC1:.yy=.hh)
# Set Global generated sources variable
GENSRC		:= $(YACCGENSRC) $(LEXGENSRC)
GENJAVA		:= $(YACCGENJAVA)
GENINC		:= $(YACCINC)
GENOBJ		:= $(YACCOBJ) $(LEXOBJ)
GENCLASSES	:= $(YACCCLASSES)

# Create find ignore pattern for generated sources and for folders containing a makefile
cmd_FIND_NOGEN	= echo $(GENSRC) $(GENINC) $(GENJAVA) \
		       "$$($(FIND) $(SRCDIR) -mindepth 2 -name 'Makefile' \
		           | $(SED) -e 's|\([^[:space:]]*\)/[^[:space:]]*|\1/*|g')" \
		  | $(SED) -e 's|\([^[:space:]]*\)|-and \! -path "\1"|g' || true
tmp_FIND_NOGEN	!= $(cmd_FIND_NOGEN)
tmp_FIND_NOGEN	?= $(shell $(cmd_FIND_NOGEN))
find_AND_NOGEN	:= $(tmp_FIND_NOGEN)

# Search non-generated sources and headers. Extensions must be in low-case.
cmd_JAVASRC	= $(FIND) $(SRCDIR) \( -name '*.java' \) $(find_AND_SYSDEP) $(find_AND_NOGEN)
# JAVASRC variable, filled from the 'find' command (cmd_JAVA) defined above.
tmp_JAVASRC	!= $(cmd_JAVASRC)
tmp_JAVASRC	?= $(shell $(cmd_JAVASRC))
JAVASRC		:= $(tmp_JAVASRC) $(GENJAVA)
JCNIINC		:= $(JAVASRC:.java=.hh)
JCNISRC		:= $(JAVASRC:.java=.cc)
GENINC		:= $(GENINC) $(JCNIINC)
CLASSES		:= $(JAVASRC:.java=.class)

# Other non-generated sources and headers. Extension must be in low-case.
# find_AND_NOGEN2	:= $(find_AND_NOGEN) `echo $(JCNIINC) | $(SED) -e 's|\([^[:space:]]*\)|-and \! -path "\1"|g'`
cmd_FIND_NOGEN2	= echo $(JCNIINC) | $(SED) -e 's|\([^[:space:]]*\)|-and \! -path "\1"|g'
tmp_FIND_NOGEN2	!= $(cmd_FIND_NOGEN2)
tmp_FIND_NOGEN2	?= $(shell $(cmd_FIND_NOGEN2))
find_AND_NOGEN2	:= $(tmp_FIND_NOGEN2)

cmd_SRC		= $(FIND) $(SRCDIR) \( -name '*.c' -or -name '*.cc' -or -name '*.cpp' -or -name '*.m' \) \
		  $(find_AND_SYSDEP) $(find_AND_NOGEN) -and \! -path '$(SRCINC)'
cmd_INCLUDES	= $(FIND) $(INCDIRS) $(SRCDIR) \( -name '*.h' -or -name '*.hh' -or -name '*.hpp' \) \
		  $(find_AND_SYSDEP) $(find_AND_NOGEN) $(find_AND_NOGEN2) -and \! -path $(VERSIONINC) -and \! -path $(BUILDINC)

# SRC variable, filled from the 'find' command (cmd_SRC) defined above.
tmp_SRC		!= $(cmd_SRC)
tmp_SRC		?= $(shell $(cmd_SRC))
SRC		:= $(SRCINC) $(tmp_SRC) $(YACCGENSRC) $(LEXGENSRC)

# OBJ variable computed from SRC, replacing SRCDIR by BUILDDIR and extension by .o
cmd_SRC_BUILD	= echo " $(SRC)" | $(SED) -e 's| $(SRCDIR)/| $(BUILDDIR)/|g'; \
		  echo "$(JAVASRC)" | $(GREP) -Eq '\.java' && echo $(BUILDDIR)/Java.o || true
tmp_SRC_BUILD	!= $(cmd_SRC_BUILD)
tmp_SRC_BUILD	?= $(shell $(cmd_SRC_BUILD))
tmp_OBJ1	= $(tmp_SRC_BUILD:.m=.o)
tmp_OBJ2	= $(tmp_OBJ1:.cpp=.o)
tmp_OBJ3	= $(tmp_OBJ2:.cc=.o)
OBJ		= $(tmp_OBJ3:.c=.o)

# INCLUDE VARIABLE, filled from the 'find' command (cmd_INCLUDES) defined above.
tmp_INCLUDES	!= $(cmd_INCLUDES)
tmp_INCLUDES	?= $(shell $(cmd_INCLUDES))
INCLUDES	:= $(VERSIONINC) $(BUILDINC) $(tmp_INCLUDES)

# Search compilers: choice might depend on what we have to build (eg: use gcc if using gcj)
cmd_GCJ		= $(WHICH) ${GCJ} gcj gcj-mp gcj-mp-6 gcj-mp-5 gcj-mp-4.9 gcj-mp-4.8 gcj-mp-4.7 gcj-mp-4.6 $(NO_STDERR) | $(HEADN1)
tmp_GCJ		!= $(cmd_GCJ)
tmp_GCJ		?= $(shell $(cmd_GCJ))
GCJ		:= $(tmp_GCJ)

cmd_CC		= $(WHICH) $$(echo "$(JAVASRC)" | $(GREP) -Eq '\.java' && echo $(GCJ) | sed -e 's|gcj\([^/]*\)|gcc\1|') \
		           $${CC} clang gcc cc $(CC) $(NO_STDERR) | $(HEADN1)
tmp_CC		!= $(cmd_CC)
tmp_CC		?= $(shell $(cmd_CC))
CC		:= $(tmp_CC)

cmd_CXX		= $(WHICH) $$($(TEST) -n "$(JAVASRC)" && echo $(GCJ) | sed -e 's|gcj\([^/]*\)|g++\1|') \
		           $${CXX} clang++ g++ c++ $(CXX) $(NO_STDERR) | $(HEADN1)
tmp_CXX		!= $(cmd_CXX)
tmp_CXX		?= $(shell $(cmd_CXX))
CXX		:= $(tmp_CXX)

cmd_GCJH	= echo "$(GCJ)" | $(SED) -e 's|gcj\([^/]*\)$$|gcjh\1|'
tmp_GCJH	!= $(cmd_GCJH)
tmp_GCJH	?= $(shell $(cmd_GCJH))
GCJH		:= $(tmp_GCJH)

# CCLD: Add '-lstdc++' if needed, use '$(GCJ) -lgcj' if needed, otherwise use $(CC)
cmd_CCLD	= echo "$(SRC) $(JAVASRC) " | $(AWK) 'BEGIN{cpp=0;gcj=0;} /\.(cc|cpp) /{cpp=1;} /\.java / {gcj=1;cpp=1;} \
		                                      END{ print (gcj?"$(GCJ) -lgcj":"$(CC)") (cpp?" -lstdc++":""); }'
tmp_CCLD	!= $(cmd_CCLD)
tmp_CCLD	?= $(shell $(cmd_CCLD))
CCLD		:= $(tmp_CCLD)

CPP		= $(CC) -E
OBJC		= $(CC)

cmd_TIMESTAMP	= $(DATE) '+%Y-%m-%d_%Hh%M'
tmp_TIMESTAMP	!= $(cmd_TIMESTAMP)
tmp_TIMESTAMP	?= $(shell $(cmd_TIMESTAMP))
TIMESTAMP	:= $(tmp_TIMESTAMP)

cmd_SHELL	:= $(WHICH) bash sh $(SHELL) $(NO_STDERR) | $(HEADN1)
tmp_SHELL	!= $(cmd_SHELL)
tmp_SHELL	?= $(shell $(cmd_SHELL))
SHELL		:= $(tmp_SHELL)

cmd_UNAME_SYS	= uname | $(TR) '[A-Z]' '[a-z]'
tmp_UNAME_SYS	!= $(cmd_UNAME_SYS)
tmp_UNAME_SYS	?= $(shell $(cmd_UNAME_SYS))
UNAME_SYS	:= $(tmp_UNAME_SYS)

cmd_UNAME_ARCH	= uname -m | $(TR) '[A-Z]' '[a-z]'
tmp_UNAME_ARCH	!= $(cmd_UNAME_ARCH)
tmp_UNAME_ARCH	?= $(shell $(cmd_UNAME_ARCH))
UNAME_ARCH	:= $(tmp_UNAME_ARCH)

# Basic yacc/bison binary search
#cmd_YACC	= which yacc bison $(NO_STDERR) | $(HEADN1)
# Search bison 3 or later, fallback on yacc, bison.
cmd_YACC	= { for bin in $$($(WHICH) -a bison $(NO_STDERR)); do \
		      ver="$$($$bin -V 2>&1 | grep -E '[0-9]+(\.[0-9]+)+' \
		                            | sed -Ee 's/.*[[:space:]]+([0-9]*\.[0-9.]*).*/\1/' \
		                            | awk -F '.' '{ print $$1*1000000 + $$2*10000 + $$3*100 + $$4*1 }')"; \
		  $(TEST) $$ver -ge 03000000 && echo $$bin && break; done; $(WHICH) yacc bison $(NO_STDERR); } | $(HEADN1)
tmp_YACC	!= $(cmd_YACC)
tmp_YACC	?= $(shell $(cmd_YACC))
YACC		:= $(tmp_YACC)

cmd_LEX		= $(WHICH) lex flex $(NO_STDERR) | $(HEADN1)
tmp_LEX		!= $(cmd_LEX)
tmp_LEX		?= $(shell $(cmd_LEX))
LEX		:= $(tmp_LEX)

JAVA		= java
JARBIN		= jar
JAVAC		= javac
JAVAH		= javah

############################################################################################

SYSDEP_SUF	= $(UNAME_SYS)
sys_LIBS	= $(LIBS_$(SYSDEP_SUF))
sys_INCS	= $(INCS_$(SYSDEP_SUF))
sys_OPTI	= $(OPTI_$(SYSDEP_SUF))
sys_WARN	= $(WARN_$(SYSDEP_SUF))
sys_DEBUG	= $(DEBUG_$(SYSDEP_SUF))

############################################################################################
# Generic Build Flags, taking care of system specific flags (sys_*)
cmd_CPPFLAGS	= { echo "-I."; $(TEST) "$(SRCDIR)" != "." && echo "-I$(SRCDIR)"; \
		    $(TEST) "$(SRCDIR)" != "$(BUILDDIR)" && echo "-I$(BUILDDIR)"; \
		    for dir in $(INCDIRS); do $(TEST) "$$dir" != "$(SRCDIR)" && $(TEST) "$$dir" != "." && echo "-I$$dir"; done; true; }
tmp_CPPFLAGS	!= $(cmd_CPPFLAGS)
tmp_CPPFLAGS	?= $(shell $(cmd_CPPFLAGS))

FLAGS_COMMON	= $(OPTI) $(WARN) $(ARCH)
CPPFLAGS	= $(tmp_CPPFLAGS) $(sys_INCS) $(INCS) $(MACROS)
CFLAGS		= $(FLAGS_C) $(FLAGS_COMMON)
CXXFLAGS	= $(FLAGS_CXX) $(FLAGS_COMMON)
OBJCFLAGS	= $(FLAGS_OBJC) $(FLAGS_COMMON)
JFLAGS		= $(FLAGS_GCJ) $(FLAGS_COMMON) $(tmp_CPPFLAGS) $(sys_INCS) $(INCS)
LDFLAGS		= $(ARCH) $(OPTI) $(sys_LIBS) $(LIBS)
ARFLAGS		= r
LFLAGS		=
LCXXFLAGS	= $(LFLAGS)
LJFLAGS		=
YFLAGS		= -d
YCXXFLAGS	= $(YFLAGS)
YJFLAGS		=
############################################################################################

ALLMAKEFILES	= Makefile
LICENSE		= LICENSE
README		= README.md
CLANGCOMPLETE	= .clang_complete
DISTNAME	= $(NAME)_$(VERSION)_$(TIMESTAMP)
SRCINC_CONTENT	= $(LICENSE) $(README) $(METASRC) $(tmp_SRC) $(tmp_JAVASRC) $(INCLUDES) $(ALLMAKEFILES)

############################################################################################
# For make recursion through sub-directories
BUILDDIRS	= $(SUBDIRS:=-build)
INSTALLDIRS	= $(SUBDIRS:=-install)
DISTCLEANDIRS	= $(SUBDIRS:=-distclean)
CLEANDIRS	= $(SUBDIRS:=-clean)
TESTDIRS	= $(SUBDIRS:=-test)
DEBUGDIRS	= $(SUBDIRS:=-debug)
DOCDIRS		= $(SUBDIRS:=-doc)

############################################################################################

# --- default_rule : build all
default_rule: update-$(BUILDINC) $(BUILDDIRS)
	@# This is not the most beautilful thing here, but the best portable stuff i found yet.
	@$(MAKE) build_all

build_all: $(BIN) $(LIB) $(JAR) gentags

$(SUBDIRS): $(BUILDDIRS)
$(BUILDDIRS):
	cd $(@:-build=) && $(MAKE)

# --- clean : remove objects and generated files
clean: $(CLEANDIRS)
	$(RM) $(OBJ:.class=*.class) $(SRCINC) $(GENSRC) $(GENINC) $(GENJAVA) $(CLASSES:.class=*.class)
$(CLEANDIRS):
	cd $(@:-clean=) && $(MAKE) clean

# --- distclean : remove objects, binaries and remove DEBUG flag in build.h
distclean: $(DISTCLEANDIRS) clean
	$(RM) $(BIN) $(LIB) $(BUILDINC) `$(FIND) . -name '.*.swp' -or -name '.*.swo' -or -name '*~' -or -name '\#*' $(NO_STDERR)`
	$(RM) -R $(BIN).dSYM || true
	@$(TEST) "$(BUILDDIR)" != "$(SRCDIR)" && $(RMDIR) `$(FIND) $(BUILDDIR) -type d | $(SORT) -r` $(NO_STDERR) || true
	@$(PRINTF) "$(NAME): distclean done, debug disabled.\n"
$(DISTCLEANDIRS):
	cd $(@:-distclean=) && $(MAKE) distclean

# --- debug : set DEBUG flag in build.h and rebuild
debug: update-$(BUILDINC) $(DEBUGDIRS)
	@{ $(GREP) -Ev '^[[:space:]]*\#[[:space:]]*define[[:space:]]+(BUILD_DEBUG|BUILD_TEST)([[:space:]]|$$)' $(BUILDINC) $(NO_STDERR); \
		$(PRINTF) "#define BUILD_DEBUG\n#define BUILD_TEST\n"; } > $(BUILDINC).tmp && $(MV) $(BUILDINC).tmp $(BUILDINC)
	@$(PRINTF) "$(NAME): debug enabled ('make distclean' to disable it).\n"
	@$(MAKE)
$(DEBUGDIRS):
	cd $(@:-debug=) && $(MAKE) debug
# Code to disable debug without deleting BUILDINC:
# @$(GREP) -Ev '^[[:space:]]*\#[[:space:]]*define[[:space:]]+(BUILD_DEBUG|BUILD_TEST)([[:space:]]|$$)' $(BUILDINC) \
#	    > $(BUILDINC).tmp && $(MV) $(BUILDINC).tmp $(BUILDINC)

# --- doc : generate doc
doc: $(DOCDIRS)
$(DOCDIRS):
	cd $(@:-doc=) && $(MAKE) doc

# --- install ---
install: $(INSTALLDIRS) all
$(INSTALLDIRS):
	cd $(@:-install=) && $(MAKE) install

# --- test ---
test: $(TESTDIRS) all
$(TESTDIRS):
	cd $(@:-test=) && $(MAKE) test

# --- build bin&lib ---
$(BIN): $(OBJ) $(SUBLIBS) $(JCNIINC)
	$(CCLD) $(OBJ:.class=*.class) $(LDFLAGS) -o $@
	@$(PRINTF) "$@: build done.\n"

$(LIB): $(OBJ) $(SUBLIBS) $(JCNIINC)
	$(AR) $(ARFLAGS) $@ $(OBJ:.class=*.class)
	$(RANLIB) $@
	@$(PRINTF) "$@: build done.\n"

# Build Java.o
$(CLASSES): $(BUILDDIR)/Java.o
	@true # Used to override implicit rule .java.class:
$(BUILDDIR)/Java.o: $(JAVASRC) $(ALLMAKEFILES)
	$(GCJ) $(JFLAGS) -d $(BUILDDIR) -C $(JAVASRC)
	$(GCJ) $(JFLAGS) -d $(BUILDDIR) -c -o $@ $(CLASSES:.class=*.class)
$(JCNISRC:.cc=.o) : $(JCNIINC)
$(JCNIINC): $(BUILDDIR)/Java.o

# This is a TODO and a TOSTUDY
$(MANIFEST):
$(JAR): $(JAVASRC) $(SUBLIBS) $(MANIFEST) $(ALLMAKEFILES)
	@echo "TODO !!"
	javac $(JAVASRC) -classpath $(SRCDIR) -d $(BUILDDIR)
	jar uf $(MANIFEST) $@ $(CLASSES:.class=*.class)
	@$(PRINTF) "$@: build done.\n"

##########################################################################################
.SUFFIXES: .o .c .h .cpp .hpp .cc .hh .m .java .class .y .l .yy .ll .yyj .llj
# OBJS are rebuilt on Makefile or headers update. Alternative: could use gcc -MD and sinclude.
$(OBJ): $(INCLUDES) $(ALLMAKEFILES)
# LEX can depend on yacc generated header: not perfect as all lex are rebuilt on yacc file update
$(LEXGENSRC): $(YACCOBJ)
# Empty rule for YACCGENSRC so that make keeps intermediate yacc generated sources
$(YACCGENSRC): $(ALLMAKEFILES)
$(YACCOBJ): $(ALLMAKEFILES)
$(YACCCLASSES): $(ALLMAKEFILES)
$(YACCGENJAVA): $(ALLMAKEFILES)
# Implicit rules: old-fashionned double suffix rules to be compatible with most make.
.m.o:
	$(OBJC) $(OBJCFLAGS) $(FLAGS_OBJC_$<) $(CPPFLAGS) -c $< -o $@
.c.o:
	$(CC) $(CFLAGS) $(FLAGS_C_$<) $(CPPFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CXXFLAGS) $(FLAGS_CXX_$<) $(CPPFLAGS) -c $< -o $@
.cc.o:
	$(CXX) $(CXXFLAGS) $(FLAGS_CXX_$<) $(CPPFLAGS) -c $< -o $@
.java.o:
	$(GCJ) $(JFLAGS) $(FLAGS_GCJ_$<) -c $< -o $@
.java.class:
	$(GCJ) $(JFLAGS) $(FLAGS_GCJ_$<) -C $<
.class.hh:
	$(GCJH) $(FLAGS_GCJH_$<) $< -o $@
.l.c:
	$(LEX) $(LFLAGS) -o $@ $<
.ll.cc:
	$(LEX) $(LCXXFLAGS) -o $@ $<
.llj.java:
	$(LEX) $(LJFLAGS) -o $@ $<
.y.c:
	$(YACC) $(YFLAGS) -o $@ $<
.yy.cc:
	$(YACC) $(YCXXFLAGS) -o $@ $<
.yyj.java:
	$(YACC) $(YJFLAGS) -o $@ $<
############################################################################################

dist:
	@$(MKDIR) -p "$(DISTDIR)/$(DISTNAME)"
	@cp -Rf . "$(DISTDIR)/$(DISTNAME)"
	@$(RM) -R `$(FIND) "$(DISTDIR)/$(DISTNAME)" -type d -and \( -name '.git' -or -name 'CVS' -or -name '.hg' -or -name '.svn' \) $(NO_STDERR)`
	@$(PRINTF) "$(NAME): building dist...\n"
	@cd "$(DISTDIR)/$(DISTNAME)" && $(MAKE) distclean && $(MAKE) && $(MAKE) distclean
	@cd "$(DISTDIR)" && { $(TAR) cJf "$(DISTNAME).tar.xz" "$(DISTNAME)" && targz=true || targz=false; \
     			      $(TAR) czf "$(DISTNAME).tar.gz" "$(DISTNAME)" || $${targz}; }
	@#cd "$(DISTDIR)" && ($(ZIP) -q -r "$(DISTNAME).zip" "$(DISTNAME)" || true)
	@$(RM) -R "$(DISTDIR)/$(DISTNAME)"
	@$(PRINTF) "$(NAME): archives created: $$(ls $(DISTDIR)/$(DISTNAME).*)\n"

$(SRCINC): $(SRCINC_CONTENT)
	@# Generate $(SRCINC) containing all sources.
	@$(PRINTF) "$(NAME): generate $@\n"
	@$(MKDIR) -p $(@D)
	@$(PRINTF) "/* generated content */\n" > $(SRCINC) ; \
		$(AWK) 'BEGIN { dbl_bkslash="\\"; gsub(/\\/, "\\\\\\", dbl_bkslash); o="awk on ubuntu 12.04"; \
	                        if (dbl_bkslash=="\\\\") dbl_bkslash="\\\\\\"; else dbl_bkslash="\\\\"; \
				print "#include <stdlib.h>\n#include \"$(VERSIONINC)\"\n#ifdef APP_INCLUDE_SOURCE\n" \
				      "static const char * const s_program_source[] = {"; } \
		   function printblk() { \
	               gsub(/\\/, dbl_bkslash, blk); \
                       gsub(/"/, "\\\"", blk); \
	               gsub(/\n/, "\\n\"\n\"", blk); \
	               print "\"" blk "\\n\","; \
	           } { \
		       if (curfile != FILENAME) { \
		           curfile="/* FILE: $(NAME)/" FILENAME " */"; blk=blk "\n" curfile; curfile=FILENAME; \
	               } if (length($$0 " " blk) > 2000) { \
	                   printblk(); blk=$$0; \
                       } else \
		           blk=blk "\n" $$0; \
		   } END { \
		       printblk(); print "NULL };\nconst char *const* $(NAME)_get_source() { return s_program_source; }\n#endif"; \
		   }' $(SRCINC_CONTENT) >> $(SRCINC) ; \
	     $(CC) -I. -c $(SRCINC) -o $(SRCINC).tmp.o $(NO_STDERR) \
	         || $(PRINTF) "#include <stdlib.h>\n#include \"$(VERSIONINC)\"\n#ifdef APP_INCLUDE_SOURCE\nstatic const char * const s_program_source[] = {\n  \"cannot include source. check awk version or antivirus or bug\\\\n\", NULL};\nconst char *const* $(NAME)_get_source() { return s_program_source; }\n#endif\n" > $(SRCINC); \
	     $(RM) -f $(SRCINC).*

$(LICENSE):
	@echo "$(NAME): create $(LICENSE)"
	@$(PRINTF) "GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007 - http://www.gnu.org/licenses/\n" > $(LICENSE)

$(README):
	@echo "$(NAME): create $(README)"
	@$(PRINTF) "## $(NAME)\n---------------\n\n* [Overview](#overview)\n* [License](#license)\n\n" > $(README)
	@$(PRINTF) "## Overview\nTODO !\n\n## License\nGPLv3 or later. See LICENSE file.\n" >> $(README)

$(VERSIONINC):
	@echo "$(NAME): create $(VERSIONINC)"
	@$(PRINTF) "#ifndef APP_VERSION_H\n#define APP_VERSION_H\n#define APP_VERSION \"0.1\"\n" > $(VERSIONINC)
	@$(PRINTF) "#define APP_INCLUDE_SOURCE\n#define APP_BUILD_NUMBER 1\n#include \"build.h\"\n#endif\n" >> $(VERSIONINC)

$(BUILDINC): $(VERSIONINC)
	@if ! $(TEST) -e $(BUILDINC); then \
	     echo "$(NAME): create $(BUILDINC)"; \
	     build=`$(AWK) '/^[[:space:]]*#define[[:space:]]APP_BUILD_NUMBER/ { print ($$3+0); }' $(VERSIONINC)`; \
	     $(PRINTF) "#define BUILD_APPNAME \"\"\n#define BUILD_NUMBER $$build\n#define BUILD_GITREV \"\"\n" > $(BUILDINC); \
	     $(PRINTF) "#define BUILD_FULLGITREV \"\"\n#define BUILD_PREFIX \"\"\n#define BUILD_SRCPATH \"\"\n" >> $(BUILDINC); \
	     $(PRINTF) "#define BUILD_CC_CMD \"\"\n#define BUILD_CXX_CMD \"\"\n#define BUILD_OBJC_CMD \"\"\n" >> $(BUILDINC); \
	     $(PRINTF) "#define BUILD_GCJ_CMD \"\"\n#define BUILD_CCLD_CMD \"\"\n" >> $(BUILDINC); \
	 fi;

update-$(BUILDINC): $(BUILDINC)
	@gitrev=`$(GIT) describe --match "v[0-9]*" --always --tags --dirty $(NO_STDERR)` \
	 && fullgitrev=`$(GIT) describe --match "v[0-9]*" --always --tags --dirty --abbrev=0 $(NO_STDERR)` \
	 || { gitrev=unknown; fullgitrev=unknown; }; \
	 if $(SED) -e "s|^\([[:space:]]*#define[[:space:]][[:space:]]*BUILD_GITREV[[:space:]]\).*|\1\"$${gitrev}\"|" \
	        -e "s|^\([[:space:]]*#define[[:space:]][[:space:]]*BUILD_FULLGITREV[[:space:]]\).*|\1\"$${fullgitrev}\"|" \
	        -e "s|^\([[:space:]]*#define[[:space:]][[:space:]]*BUILD_PREFIX[[:space:]]\).*|\1\"$(PREFIX)\"|" \
	        -e "s|^\([[:space:]]*#define[[:space:]][[:space:]]*BUILD_SRCPATH[[:space:]]\).*|\1\"$(TOPDIR)\"|" \
	        -e "s|^\([[:space:]]*#define[[:space:]][[:space:]]*BUILD_APPNAME[[:space:]]\).*|\1\"$(NAME)\"|" \
	        -e "s|^\([[:space:]]*#define[[:space:]][[:space:]]*BUILD_CC_CMD[[:space:]]\).*|\1\"$(CC) $(CFLAGS) $(CPPFLAGS) -c\"|" \
	        -e "s|^\([[:space:]]*#define[[:space:]][[:space:]]*BUILD_CXX_CMD[[:space:]]\).*|\1\"$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c\"|" \
	        -e "s|^\([[:space:]]*#define[[:space:]][[:space:]]*BUILD_OBJC_CMD[[:space:]]\).*|\1\"$(OBJC) $(OBJCFLAGS) $(CPPFLAGS) -c\"|" \
	        -e "s|^\([[:space:]]*#define[[:space:]][[:space:]]*BUILD_GCJ_CMD[[:space:]]\).*|\1\"$(GCJ) $(JFLAGS) $(CPPFLAGS) -c\"|" \
	        -e "s|^\([[:space:]]*#define[[:space:]][[:space:]]*BUILD_CCLD_CMD[[:space:]]\).*|\1\"$(CCLD) $(LDFLAGS)\"|" \
	        $(BUILDINC) > $(BUILDINC).tmp; then \
	    if $(DIFF) $(BUILDINC) $(BUILDINC).tmp $(NO_STDOUT); then $(RM) $(BUILDINC).tmp; \
            else $(MV) $(BUILDINC).tmp $(BUILDINC) && echo "$(NAME): $(BUILDINC) updated"; fi; \
	 fi

.gitignore: $(ALLMAKEFILES)
	@{ cat .gitignore $(NO_STDERR); for f in $(BIN) $(BIN).dSYM $(LIB) $(JAR) $(GENSRC) $(GENJAVA) $(GENINC) $(SRCINC) $(BUILDINC) $(CLANGCOMPLETE) \
	                                         '*.o' '*.class' '*.a' '*~' '.*.swo' '.*.swp'; do \
	    $(PRINTF) "$$f\n" | $(SED) -e 's|^./||'; done; } \
	 | $(SORT) | $(UNIQ) > .gitignore

gentags: $(CLANGCOMPLETE)
$(CLANGCOMPLETE): $(ALLMAKEFILES)
	@echo "$(NAME): update $(CLANGCOMPLETE)"
	@src=`echo $(SRCDIR) | $(SED) -e 's|\.|\\\.|g'`; $(TEST) -e $(CLANGCOMPLETE) \
		&& $(SED) -e "s%^[^#]*-I$$src[[:space:]].*%$(CPPFLAGS) %" -e "s%^[^#]*-I$$src$$%$(CPPFLAGS)%" \
	             "$(CLANGCOMPLETE)" $(NO_STDERR)  > "$(CLANGCOMPLETE).tmp" \
	        && mv "$(CLANGCOMPLETE).tmp" "$(CLANGCOMPLETE)" \
	    || echo "$(CPPFLAGS)" > $(CLANGCOMPLETE)

info:
	@echo "UNAME_SYS        : $(UNAME_SYS)"
	@echo "UNAME_ARCH       : $(UNAME_ARCH)"
	@echo "MAKE             : $(MAKE)"
	@echo "SHELL            : $(SHELL)"
	@echo "FIND             : $(FIND)"
	@echo "AWK              : $(AWK)"
	@echo "GREP             : $(GREP)"
	@echo "SED              : $(SED)"
	@echo "TAR              : $(TAR)"
	@echo "DATE             : $(DATE)"
	@echo "PKGCONFIG        : $(PKGCONFIG)"
	@echo "CC               : $(CC)"
	@echo "CXX              : $(CXX)"
	@echo "OBJC             : $(OBJC)"
	@echo "GCJ              : $(GCJ)"
	@echo "GCJH             : $(GCJH)"
	@echo "CPP              : $(CPP)"
	@echo "CCLD             : $(CCLD)"
	@echo "YACC             : $(YACC)"
	@echo "LEX              : $(LEX)"
	@echo "CFLAGS           : $(CFLAGS)"
	@echo "CXXFLAGS         : $(CXXFLAGS)"
	@echo "OBJCFLAGS        : $(OBJCFLAGS)"
	@echo "JFLAGS           : $(JFLAGS)"
	@echo "CPPFLAGS         : $(CPPFLAGS)"
	@echo "LDFLAGS          : $(LDFLAGS)"
	@echo "YFLAGS           : $(YFLAGS)"
	@echo "YCXXFLAGS        : $(YCXXFLAGS)"
	@echo "YJFLAGS          : $(YJFLAGS)"
	@echo "LFLAGS           : $(LFLAGS)"
	@echo "LCXXFLAGS        : $(LCXXFLAGS)"
	@echo "LJFLAGS          : $(LJFLAGS)"
	@echo "SRCDIR           : $(SRCDIR)"
	@echo "DISTDIR          : $(DISTDIR)"
	@echo "BUILDDIR         : $(BUILDDIR)"
	@echo "DISTNAME         : $(DISTNAME)"
	@echo "PREFIX           : $(PREFIX)"
	@echo "BIN              : $(BIN)"
	@echo "LIB              : $(LIB)"
	@echo "METASRC          : $(METASRC)"
	@echo "GENINC           : $(GENINC)"
	@echo "GENSRC           : $(GENSRC)"
	@echo "GENJAVA          : $(GENJAVA)"
	@echo "INCLUDES         : $(INCLUDES)"
	@echo "SRC              : $(SRC)"
	@echo "JAVASRC          : $(JAVASRC)"
	@echo "OBJ              : $(OBJ)"
	@echo "CLASSES          : $(CLASSES)"

.PHONY: subdirs $(SUBDIRS)
.PHONY: subdirs $(BUILDDIRS)
.PHONY: subdirs $(INSTALLDIRS)
.PHONY: subdirs $(TESTDIRS)
.PHONY: subdirs $(CLEANDIRS)
.PHONY: subdirs $(DISTCLEANDIRS)
.PHONY: subdirs $(DEBUGDIRS)
.PHONY: subdirs $(DOCDIRS)
.PHONY: default_rule all build_all clean distclean dist test info debug doc install gentags update-$(BUILDINC)

############################################################################
#$(BUILDDIR)/%.o: $(SRCDIR)/%.m $(INCLUDES) $(ALLMAKEFILES)
#	@$(MKDIR) -p $(@D)
#	$(OBJC) -c $< $(OBJCFLAGS) $(CPPFLAGS) -o $@

#$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES) $(ALLMAKEFILES)
#	@$(MKDIR) -p $(@D)
#	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@

#$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDES) $(ALLMAKEFILES)
#	@$(MKDIR) -p $(@D)
#	$(CXX) -c $< $(CXXFLAGS) $(CPPFLAGS) -o $@

#$(BUILDDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDES) $(ALLMAKEFILES)
#	@$(MKDIR) -p $(@D)
#	$(CXX) -c $< $(CXXFLAGS) $(CPPFLAGS) -o $@