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
 * test c++ api called by c
 */
#include <iostream>
extern "C" int c_call_for_cpp(int a);

extern "C" int cpp_call_for_c(int a) {
    std::cout << "cpp call for c " << a << std::endl;
    if (a < 3)
        return c_call_for_cpp(a+1);
    return a;
}

extern "C" int jcni();
extern "C" int cpp_cni_call_for_c(int a) {
    std::cout << "cpp cni call for c " << a << std::endl;
    //call JCni java cni interface
#   ifdef _NOJAVA
    return 0;
#   else
    return jcni();
#   endif
}
