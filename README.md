
## vmultilangdemo
-----------------

* [Overview](#overview)
* [System Requirments](#systemrequirments)
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
Make sure you clone the repository with '--recursive' option.  
    $ git clone --recursive https://github.com/vsallaberry/vmultilangdemo

Just type:  
    $ make # (or 'make -j3' for SMP)

If the Makefile cannot be parsed by 'make', try:  
    $ ./make-fallback

Most of utilities used in Makefile are defined in variables and can be changed
with something like 'make SED=gsed TAR=gnutar' (or ./make-fallback SED=...)

To See how make understood the Makefile, you can type:  
    $ make info # ( or ./make-fallback info)

When making without version.h created (not the case for this repo), some old
bsd make can stop. Just type again '$ make' and it will be fine.

## Contact
[vsallaberry@gmail.com]  
<https://github.com/vsallaberry/vmultilangdemo>

## License
GPLv3 or later. See LICENSE file.

CopyRight: Copyright (C) 2018 Vincent Sallaberry

