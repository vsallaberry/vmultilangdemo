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
 * jni native method implentation for JMain::nat_main(). NOT USED if cni.
 */
#include "version.h"
#if BUILD_JAR
#include <jni.h>
#include <stdio.h>

JNIEXPORT void JNICALL JMain_nat_1main (JNIEnv * env, jclass clazz) {
    fprintf(stdout, "hello from jni\n");
    int cpp_call_for_c(int a);
    cpp_call_for_c(0); // call c++ utility.
    (void)env;
    (void)clazz;
}
#else
extern void unused();
#endif

