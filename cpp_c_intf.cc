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
 * test c++ api called by c
 */
#include <iostream>
#include <climits>

#include "version.h"

extern "C" int c_call_for_cpp(int a);

extern "C" int cpp_call_for_c(int a) {
    std::cout << "  + [" << __FILE__ << "] cpp call for c " << a << std::endl;
    int sz = (a+1) * 12 - 51 * (a+4);
    if (a == 0) {
        //size_t s = ULONG_MAX;
        //int * p = NULL;
        try {
            std::cout << "  + [" << __FILE__ << "] throwing c++ exception..." << std::endl;
            int * p = new int[sz];
            // should not be reached
            a = 12;
            std::cout << "  + [" << __FILE__ << "] pointer = " << p << std::endl;
        } catch (std::bad_alloc) {
            std::cout << "  + [" << __FILE__ << "] caught std::bad_alloc exception." << std::endl;
        } catch (...) {
            std::cout << "  + [" << __FILE__ << "] caught unknown exception." << std::endl;
        }
    }
    if (a < 3)
        return c_call_for_cpp(a+1);
    return a;
}

extern "C" int jcni();
extern "C" int cpp_cni_call_for_c(int a) {
    std::cout << "  + [" << __FILE__ << "] cpp cni call for c input:" << a << std::endl;
    //call JCni java cni interface
#   if BUILD_JAVAOBJ
    return jcni();
#   else
    return 0;
#   endif
}

