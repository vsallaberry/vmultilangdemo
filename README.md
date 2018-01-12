
## vmultilangdemo
-----------------
Multiple language/meta-language demo program.

* [Overview](#overview)
* [System Requirments](#systemrequirments)
* [Compilation](#compilation)
* [Contact](#contact)
* [License](#license)

## Overview
**vmultilangdemo** is a multiple language/meta-language demo program
(c/c++/java/objc/bison/flex).

NOTE:
- This is a work in progress, this program is not no fully operational yet.
- improvements will be done to take care of disabled features

## System requirements
- A somewhat capable compiler (gcc/clang), make (GNU,BSD), sh (sh/bash/ksh)
  and coreutils (awk,grep,sed,date,touch,head,printf,which,find,test,...)
- For all Java features: need bison >=3, gcj, gcc, g++.

This is not an exhaustive list but the list of systems on which it has been built:
- Linux: ubuntu 12.04 3.11.0 (without bison3 java .yyj)
  $ make GCJH='gcjh -I.' LIBS='-lstdc++' YACCJAVA=
- OSX 10.11.6
  With bison3: $ make
  Without    : $ make YACCJAVA=
- OpenBSD 5.5 except java and flex, bison, bison3
  $ make YACCJAVA= JAVASRC= YACCSRC= LEXSRC= MACROS='-D_NOJAVA'
- FreeBSD 11.1 except java, bison, flex, bison3
  $ make YACCJAVA= JAVASRC= YACCSRC= LEXSRC= MACROS='-D_NOJAVA'

## Compilation
Make sure you clone the repository with '--recursive' option.
    $ git clone --recursive https://github.com/vsallaberry/vmultilangdemo

Just type:
    $ make

If the Makefile cannot be parsed by 'make', try:
    $ ./make-fallback

Most of utilities used in Makefile are defined in variables and can be changed
with something like 'make SED=gsed TAR=gnutar' (or ./make-fallback SED=...)

To See how make understood the Makefile, you can type:
    $ make info # ( or ./make-fallback info)

## Contact
[vsallaberry@gmail.com]
<https://github.com/vsallaberry/vmultilangdemo>

## License
GPLv3 or later. See LICENSE file.

CopyRight: Copyright (C) 2018 Vincent Sallaberry

