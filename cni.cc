/*
 * Copyright (C) 2018 Vincent Sallaberry
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
#ifndef _NOJAVA
#include <iostream>
#include <java/lang/Object.h>
#include <gcj/array.h>
// Declare JCni cni interface: should be output of 'gcjh -cni JCni.class'
extern "Java" class JCni : public ::java::lang::Object {
  public:
    JCni();
    void jcni();
    static const jint JCNI_VER = 1;
    static ::java::lang::Class class$;
};

extern "C" int jcni() {
    static JCni *j = new JCni();
    j->jcni();
    //delete j;
    return 0;
}

//Declare JMain cni interfaces: should be output of 'gcjh -cni JMain.class'
extern "Java" class JMain : public ::java::lang::Object {
  public:
    JMain();
    static void main(JArray < ::java::lang::String * > *);
    static void jmain();
    static void nat_main();
    static const jint JMAIN_VER = 1;
    static ::java::lang::Class class$;
};

extern "C" int cpp_call_for_c(int);

// JMain::nat_main() java native method implementation.
extern "Java" void JMain::nat_main() {
    /*void JMain_nat_1main (void*, void*);
    JMain_nat_1main(NULL, NULL);*/
    std::cout << "hello from cni native JMain::nat_main()" << std::endl;
    cpp_call_for_c(0); // call c++ utility.
}
#endif