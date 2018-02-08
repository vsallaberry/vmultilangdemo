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
 * main calling, c, cpp, java, parse (lex c, lex cpp, yacc c, bison java).
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cmath>
#include <csignal>
#include <ctype.h>

#include "version.h"
#if BUILD_JAVAOBJ
# include <gcj/cni.h>
# if BUILD_BISON3
#  include "Parser.hh"
# endif
#endif

#if BUILD_YACC && BUILD_LEX
# include "bison_compat.h"
BCOMPAT_PARSER_DECL(y0);
BCOMPAT_PARSER_DECL(y1);
BCOMPAT_PARSER_DECL(y2);
# ifdef BUILD_SYS_linux
BCOMPAT_PARSER_DECL(y0linux_);
# endif
typedef int (*parsefun_t)(void *, void *);
typedef struct {
    parsefun_t      parsefun;
    const void *    content; /* content for parsestr, filepath for parsefile, pfile for parsefileptr */
    /* no union specific initializer in c++98 -> inline union */
    long            lexpected; /* put <> INT_MAX to consider output as long, INT_MIN to not test expected */
    double          dexpected; /* put <> HUGE_VAL to consider output as double, -HUGE_VAL to not test expected */
    const char *    sexpected; /* put <> NULL to consider output as string */
    const char *    title;
} parse_test_t;

# define Y0_DESC     "simple arithmetic ('1', '2*(-1+3)', ...)."
# define Y1_DESC     "`exp: float | (exp) | cos(exp);` (eg: '1,0', 'cos(0.0))."
# define Y2_DESC     "`exp: float | (exp) | sin(exp) | tan(exp);` (eg: '1,0', 'sin(0.0))."
# define PF(x)  ((parsefun_t)(x))
static const parse_test_t parse_tests[] = {
    { PF(y0parsefile),      NULL,               INT_MIN, HUGE_VAL, NULL,    "1.1) enter " Y0_DESC },
    { PF(y0parsefileptr),   stdin,              INT_MIN, HUGE_VAL, NULL,    "1.2) enter " Y0_DESC },
    { PF(y1parsefile),      NULL,               INT_MAX, -HUGE_VAL, NULL,   "2.1) enter " Y1_DESC },
    { PF(y1parsefile),      NULL,               INT_MAX, -HUGE_VAL, NULL,   "2.2) enter " Y1_DESC },
    { PF(y2parsefile),      NULL,               INT_MAX, -HUGE_VAL, NULL,   "3.1) enter " Y2_DESC },
    { PF(y2parsefileptr),   stdin,              INT_MAX, -HUGE_VAL, NULL,   "3.2) enter " Y2_DESC },
    { PF(y0parsestr),       " 2  *(-1+3  )",    4, HUGE_VAL, NULL,          "1.3) [buffer] " Y0_DESC },
    { PF(y1parsestr),       " cos( 0,0) ",      INT_MAX, 1.0, NULL,         "2.3) [buffer] " Y1_DESC },
    { PF(y1parsestr),       " 3.14 ",           INT_MAX, 3.14, NULL,        "2.4) [buffer] " Y1_DESC },
    { PF(y2parsestr),       " sin( 0,0) ",      INT_MAX, 0.0, NULL,         "3.3) [buffer] " Y2_DESC },
    { PF(y2parsestr),       " 3.14 ",           INT_MAX, 3.14, NULL,        "3.4) [buffer] " Y2_DESC },
    { PF(y0parsestr),       " -999 % 1000",     -999, HUGE_VAL, NULL,       "1.4) [buffer] " Y0_DESC },
    { NULL, NULL, 0, 0.0, NULL, NULL },
};
static bool sisprint(const char * s) {
    while (*s && isprint(*s)) s++;
    return *s == 0;
}
static bool test_parser(const parse_test_t * test, FILE * out, unsigned int * nerrors, unsigned int * nok);
#endif // if BUILD_LEX && BUILD_YACC

extern "C" const char *const* vmultilangdemo_get_source();
extern "C" int m();
extern "C" int cpp_call_for_c(int);
extern "C" int cpp_cni_call_for_c(int);

/** global running state used by signal handler */
static sig_atomic_t s_running = 1;
/** signal handler * /
static void sig_handler(int sig) {
    if (sig == SIGINT)
        s_running = 0;
}
*/

int main(int argc, const char *const* argv) {
    unsigned int    nerrors = 0, nok = 0;
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

    // install SIG_INT handler
    //signal(SIGINT, sig_handler);

    // Run cpp/c/objc test code
    fprintf(stdout, "\n** running c/cpp/obj sample code\n");
    //m();
    if ((ret = cpp_call_for_c(0)) != 3) {
        nerrors++;
    } else
        nok++;
    fprintf(stdout, "cpp_call_for_c(0) --> %d\n", ret);

    // Run Java Code if included in build
#  if BUILD_JAVAOBJ
    fprintf(stdout, "\n** Starting Java...\n");
    JvCreateJavaVM(NULL);
    JvAttachCurrentThread(NULL, NULL);
    if ((ret = cpp_cni_call_for_c(0)) != 3) {
        nerrors++;
    } else
        nok++;
    fprintf(stdout, "cpp_cni_call_for_c(0) --> %d\n", ret);
#   if BUILD_BISON3
    Parser * jparser = new Parser(NULL);
    jparser->setDebugLevel(965);
    ret = (int) jparser->getDebugLevel();
    fprintf(stdout, "javaparser addr:%p debuglevel:%d\n", (void*)jparser, ret);
    if (ret == 965) nok++; else nerrors++;
    try {
        //jparser->parse();
    } catch (...) {
        fprintf(stdout, "java exception\n");
    }
#   endif // if BUILD_BISON3
    JvDetachCurrentThread();
#  endif // if BUILD_JAVAOBJ

    // Run lex/yacc if included in build.
#   if BUILD_LEX && BUILD_YACC
    fprintf(stdout, "\n** Running yacc/lex samples\n");

    for (const parse_test_t * test = parse_tests; s_running && test && test->parsefun; test++) {
        test_parser(test, stdout, &nerrors, &nok);
    }
    y0parsedestroy();
    y1parsedestroy();
    y2parsedestroy();
#   endif

    fprintf(stdout, "\n** %u error(s) (%d tests)\n", nerrors, nerrors + nok);
    return nerrors;
}

#if BUILD_LEX && BUILD_YACC
static bool test_parser(const parse_test_t * test, FILE * out, unsigned int * nerrors, unsigned int * nok) {
    char            result[4096];
    void *          presult = result;
    int             ret;

    if (test == NULL || out == NULL || nerrors == NULL || nok == NULL) {
        fprintf(stderr, "test_parser: input error: test=%p out=%p err=%p nok=%p\n",
                (void*)test, (void*)out, (void*)nerrors, (void*)nok);
        if (nerrors) (*nerrors)++;
        return false;
    }
    fprintf(out, "\n%s\n> ", test->title);
    if (test->content != NULL && sisprint((const char*)test->content)) {
        fprintf(out, "%s\n", (const char*)test->content);
    }
    if ((ret = test->parsefun((void*)test->content, &result)) != 0) {
        (*nerrors)++;
        fprintf(out, "error, <yy>process(): %d\n", ret);
    } else {
        char check = CHAR_MIN+4;
        if (test->lexpected != INT_MAX) {
            long lresult = *((long*)presult);
            fprintf(out, "  = %ld", lresult);
            if (test->lexpected != INT_MIN)
                check = (lresult == test->lexpected);
        } else if (test->dexpected != HUGE_VAL) {
            double dresult = *((double*)presult);
            fprintf(out, "  = %f", dresult);
            if (test->dexpected != -HUGE_VAL)
                check = (dresult == test->dexpected);
        } else if (test->sexpected != NULL) {
            strcpy(result+sizeof(result)-4, "...");
            fprintf(out, "  = %s", result);
            check = !strcmp(result, test->sexpected);
        } else {
            fprintf(out, "  = ???");
        }
        if (check != CHAR_MIN+4) {
            if (!check) { (*nerrors)++; fputs(" !! [KO]", out); }
            else { (*nok)++; fputs(" [OK]", out); }
        } else
            (*nok)++;
        fputc('\n', out);
    }
    return *nerrors == 0;
}
#endif // if BUILD_LEX && BUILD_YACC

