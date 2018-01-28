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
 * main
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "version.h"
#if BUILD_JAVAOBJ
# include <gcj/cni.h>
#endif

#if BUILD_YACC
# include "bison_compat.h"
BCOMPAT_PARSER_DECL(y0);
BCOMPAT_PARSER_DECL(y1);
#endif

extern "C" const char *const* vmultilangdemo_get_source();
extern "C" int m();
extern "C" int cpp_call_for_c(int);
extern "C" int cpp_cni_call_for_c(int);

int main(int argc, const char *const* argv) {
#   if BUILD_LEX && BUILD_YACC
    const char *    parsestr;
    double          dresult, dexpected;
    long            lresult, lexpected;
#   endif
    unsigned int    nerrors = 0;
    int             ret;

    fprintf(stdout, "%s (%s v%s built on %s, %s from git-rev %s)\n\n",
            *argv, BUILD_APPNAME, APP_VERSION, __DATE__, __TIME__, BUILD_GITREV);

    fprintf(stdout, "Copyright (C) 2018 Vincent Sallaberry.\n"
                    "This is free software; see the source for copying conditions.  There is NO\n"
                    "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n");
    fprintf(stdout, "  prefix     : %s\n"
                    "  srcpath    : %s\n"
                    "  git_full   : %s\n"
                    "  CC         : %s\n"
                    "  CXX        : %s\n"
                    "  GCJ        : %s\n"
                    "  CCLD       : %s\n"
                    "  flags      : LEX:%d YACC:%d BISON3:%d JAVAOBJ:%d BIN:%d LIB:%d JAR:%d"
#                 ifdef BUILD_DEBUG
                    " DEBUG"
#                 endif
#                 ifdef BUILD_TEST
                    " TEST"
#                 endif
                    "\n",
                    BUILD_PREFIX, BUILD_SRCPATH, BUILD_FULLGITREV,
                    BUILD_CC_CMD, BUILD_CXX_CMD, BUILD_GCJ_CMD, BUILD_CCLD_CMD,
                    BUILD_LEX, BUILD_YACC, BUILD_BISON3, BUILD_JAVAOBJ, BUILD_BIN, BUILD_LIB, BUILD_JAR);
    if (argc > 1 && !strcmp(argv[1], "-s")) {
#     ifdef APP_INCLUDE_SOURCE
        for (const char *const* line = vmultilangdemo_get_source(); *line; line++) {
            fprintf(stdout, "%s", *line);
        }
#     endif
    }

    // Run cpp/c/objc test code
    fprintf(stdout, "\n** running c/cpp/obj sample code\n");
    //m();
    if ((ret = cpp_call_for_c(0)) != 3) {
        nerrors++;
    }
    fprintf(stdout, "cpp_call_for_c(0) --> %d\n", ret);

    // Run Java Code if included in build
#   if BUILD_JAVAOBJ
    fprintf(stdout, "\n** Starting Java...\n");
    JvCreateJavaVM(NULL);
    JvAttachCurrentThread(NULL, NULL);
    if ((ret = cpp_cni_call_for_c(0)) != 3) {
        nerrors++;
    }
    fprintf(stdout, "cpp_cni_call_for_c(0) --> %d\n", ret);
    JvDetachCurrentThread();
#   endif

    // Run lex/yacc if included in build.
#   if BUILD_LEX && BUILD_YACC
    fprintf(stdout, "\n** Running yacc/lex samples\n");

    /* parse-test: stdin */
    fprintf(stdout, "1.1) enter simple arithmetic ('1', '2*(-1+3)', ...).\n> ");
    if ((ret = y0parsefile(NULL, &lresult)) != 0) {
        nerrors++;
        fprintf(stdout, "error, y0process(): %d\n", ret);
    } else fprintf(stdout, "  = %ld\n", lresult);

    /* parse-test: stdin */
    fprintf(stdout, "1.2) enter simple arithmetic ('1', '2*(-1+3)', ...).\n> ");
    if ((ret = y0parsefile(NULL, &lresult)) != 0) {
        nerrors++;
        fprintf(stdout, "error, y0process(): %d\n", ret);
    } else fprintf(stdout, "  = %ld\n", lresult);

    /* parse-test2: stdin */
    fprintf(stdout, "2.1) enter `exp: = float | (exp) | cos(exp);` (eg: '1,0', 'cos(0.0)).\n> ");
    if ((ret = y1parsefile(NULL, &dresult)) != 0) {
        nerrors++;
        fprintf(stdout, "error y1process(): %d\n", ret);
    } else fprintf(stdout, "  = %lf\n", dresult);

    /* parse-test2: stdin */
    fprintf(stdout, "2.2) enter `exp: = float | (exp) | cos(exp);` (eg: '1,0', 'cos(0.0)).\n> ");
    if ((ret = y1parsefile(NULL, &dresult)) != 0) {
        nerrors++;
        fprintf(stdout, "error y1process(): %d\n", ret);
    } else fprintf(stdout, "  = %lf\n", dresult);

    /* parse-test: buffer */
    parsestr = " 2  *(-1+3  )";
    lexpected = 4;
    fprintf(stdout, "1.3) simple arithmetic ('1', '2*(-1+3)', ...).\n> %s\n", parsestr);
    if ((ret = y0parsestr(parsestr, &lresult)) != 0) {
        nerrors++;
        fprintf(stdout, "error, y0process(): %d\n", ret);
    } else {
        fprintf(stdout, "  = %ld\n", lresult);
        if (lresult != lexpected) nerrors++;
    }

    /* parse-test2: buffer */
    parsestr = " cos( 0,0) ";
    dexpected = 1.0;
    fprintf(stdout, "2.3) exp: = float | (exp) | cos(exp); (eg: '1,0', 'cos(0.0)).\n> %s\n", parsestr);
    if ((ret = y1parsestr(parsestr, &dresult)) != 0) {
        nerrors++;
        fprintf(stdout, "error, y1process(): %d\n", ret);
    } else {
        fprintf(stdout, "  = %lf\n", dresult);
        if (dresult != dexpected) nerrors++;
    }

    /* parse-test2: buffer */
    parsestr = " 3.14 ";
    dexpected = 3.14;
    fprintf(stdout, "2.3) exp: = float | (exp) | cos(exp); (eg: '1,0', 'cos(0.0)).\n> %s\n", parsestr);
    if ((ret = y1parsestr(parsestr, &dresult)) != 0) {
        nerrors++;
        fprintf(stdout, "error, y1process(): %d\n", ret);
    } else {
        fprintf(stdout, "  = %lf\n", dresult);
        if (dresult != dexpected) nerrors++;
    }
#   endif

    fprintf(stdout, "\n** %u error(s)\n", nerrors);
    return nerrors;
}
