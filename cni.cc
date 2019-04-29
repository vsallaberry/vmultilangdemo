/*
 * Copyright (C) 2018-2019 Vincent Sallaberry
 * vmultilangdemo <https://github.com/vsallaberry/vmultilangdemo>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * -------------------------------------------------------------------------
 * cni gcj java/c++ interface. allows calls java->c++ and c++->java.
 */
#include "version.h"
#if BUILD_JAVAOBJ
#include <iostream>

#include <gcj/cni.h>
#include "JCni.hh"
#include "JMain.hh"

#include <java/lang/String.h>

#include <cstdlib>

extern "C" int jcni() {
    static JCni *j = new JCni();
    int ret;

    ret = j->jcni();
    //delete j;
    return ret;
}

extern "C" int cpp_call_for_c(int);
extern "C" int cc_main(int, char **);

// JMain::nat_main() java native method implementation.
extern "Java" jint JMain::nat_main() {
    /*void JMain_nat_1main (void*, void*);
    JMain_nat_1main(NULL, NULL);*/
    std::cout << "  + [" << __FILE__ << "] hello from cni native JMain::nat_main()" << std::endl;
    return cpp_call_for_c(0); // call c++ utility.
}

// JMain::cc_main() java native method implementation, used when Main is in Java instead of c++.
//   -> transform String[] args to (argc,argv) and call c++ main.
extern "Java" jint JMain::cc_main(JArray < java::lang::String * > * args) {
    int argc = args->length + 1;
    char ** argv = (char**) malloc(sizeof(char *) * (argc + 1));

    std::cout << "  + [" << __FILE__ << "] hello from cni native JMain::cc_main(nb_args:" << argc -  1 << ")" << std::endl;
    argv[0] = strdup("JAVA-" BUILD_APPNAME);
    argv[argc] = NULL;
    for (int i = 1; i < argc; i++) {
        int         j;
        jchar *     jchars  = _Jv_GetStringChars(elements(args)[i-1]);
        int         len     = elements(args)[i-1]->length();

        argv[i] = (char *) malloc(len + 1);
        for (j=0; j < len; j++) {
            argv[i][j] = jchars[j];
        }
        argv[i][j] = 0;
    }
    return ::cc_main(argc, argv);
}
#endif
