/*
 * Copyright (C) 2019 Vincent Sallaberry
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
#ifndef BUILD_GNAT
extern int ___dummy;
#else
extern void ada_adatest();
extern int ada_adatest2(int);
//void adainit();
//void adafinal();

/* function calling ada method */
static int ani(int a) {
    int ret;
    //adainit();
    ada_adatest();
    ret = ada_adatest2(a);
    //adafinal();
    return ret;
}

/* c function called by ada */
int c_util_for_ada2(int a) {
    fprintf(stdout, "      + [%s] %s(): input a:%d\n", __FILE__, __func__, a);
    return a * a - 1;
}

/* exported c method for main, calling ada */
int ada_call_for_c(int a) {
    fprintf(stdout, "  + [%s] %s\n", __FILE__, __func__);
    return ani(a);
}
#endif
