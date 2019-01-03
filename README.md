
## vmultilangdemo
-----------------

* [Overview](#overview)
* [System Requirements](#system-requirements)
* [Compilation](#compilation)
* [Contact](#contact)
* [License](#license)

## Overview
**vmultilangdemo** is a multiple language/meta-language demo program
(c/c++/java/objc/bison/flex).

NOTE:
- This is a work in progress, this program is not fully operational yet.
- Sorry for the disorder in file structure, the goal is to check different
configurations for file locations.

## System requirements
- A somewhat capable compiler (gcc/clang), make (GNU,BSD), sh (sh/bash/ksh)
  and coreutils (awk,grep,sed,date,touch,head,printf,which,find,test,...)
- For native java features: need bison >=3, gcj, gcc, g++.

This is not an exhaustive list but the list of systems on which it has been built:
- Linux slitaz 4 2.6.37 (without flex,yacc,bison3,java)
- Linux ubuntu 12.04 3.11.0 (without bison3, with 'apt-get install gcj')
- Linux debian9 (with 'apt-get install gcj-6')
- OSX 10.11.6  (with gcc/g++/gcj v6 and bison v3)
- OpenBSD 5.5 except java and bison3.
- FreeBSD 11.1 except java and bison3.

## Compilation

### Cloning **vmultilangdemo** repository
**vmultilangdemo** is using SUBMODROOTDIR Makefile's feature (RECOMMANDED, see [submodules](#using-git-submodules)):  
    $ git clone https://github.com/vsallaberry/vmultilangdemo.git  
    $ git submodule update --init # or just 'make'  

Otherwise:  
    $ git clone --recursive https://vsallaberry/vmultilangdemo.git  

### Building
Just type:  
    $ make # (or 'make -j3' for SMP)  

If the Makefile cannot be parsed by 'make', try:  
    $ ./make-fallback  

### General information
An overview of Makefile rules can be displayed with:  
    $ make help  

Most of utilities used in Makefile are defined in variables and can be changed
with something like 'make SED=gsed TAR=gnutar' (or ./make-fallback SED=...)  

To See how make understood the Makefile, you can type:  
    $ make info # ( or ./make-fallback info)  

When making without version.h created (not the case for this repo), some old
bsd make can stop. Just type again '$ make' and it will be fine.  

### Using git submodules
When your project uses git submodules, it is a good idea to group
submodules in a common folder, here, 'ext'. Indeed, instead of creating a complex tree
in case the project (A) uses module B (which uses module X) and module C (which uses module X),
X will not be duplicated as all submodules will be in ext folder.  

You need to set the variable SUBMODROOTDIR in your program's Makefile to indicate 'make'
where to find submodules (will be propagated to SUBDIRS).  

As SUBDIRS in Makefile are called with SUBMODROOTDIR propagation, currently you cannot use 
'make -C <subdir>' (or make -f <subdir>/Makefile) but instead you can use 'make <subdir>',
 'make {test,test-build,install,debug,...}-<subdir>', as <subdir>, test-<subdir>, ... are
defined as targets.  

When SUBMODROOTDIR is used, submodules of submodules will not be populated as they are
included in root project. The command `make subsubmodules` will update index of non-populated 
sub-submodules to the index used in the root project.

You can let SUBMODROOTDIR empty if you do not want to group submodules together.

## Contact
[vsallaberry@gmail.com]  
<https://github.com/vsallaberry/vmultilangdemo>

## License
GPLv3 or later. See LICENSE file.

CopyRight: Copyright (C) 2018-2019 Vincent Sallaberry

