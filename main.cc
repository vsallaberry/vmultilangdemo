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
 * main calling, c, cpp, java, parse (lex c, lex cpp, yacc c, bison java).
 */
#include "version.h"
#if BUILD_JAVAOBJ
# include <gcj/cni.h>
# include <gcj/array.h>
# include <java/lang/Exception.h>
# include <java/lang/NullPointerException.h>
# include <java/lang/String.h>
# include <JMain.hh>
# if BUILD_BISON3
#  include "Parser.hh"
# endif
#endif

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cmath>
#include <csignal>
#include <ctype.h>

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
    int             yyparse_expected; /* expected result of <yy>parse<*>() */
    /* no union specific initializer in c++98 -> inline union */
    long            lexpected; /* put <> INT_MAX to consider output as long, INT_MIN to not test expected */
    double          dexpected; /* put <> HUGE_VAL to consider output as double, -HUGE_VAL to not test expected */
    const char *    sexpected; /* put <> NULL to consider output as string */
    const char *    title;
} parse_test_t;

# define Y0_DESC     "simple arithmetic ('1', '2*(-1+3)', ...)."
# define Y1_DESC     "`exp: float | (exp) | cos(exp);` (eg: '1,0', 'cos(0.0))."
# define Y2_DESC     "`exp: float | (exp) | sin(exp) | tan(exp);` (eg: '1,0', 'sin(0.0))."
# define Y0L_DESC    "dummy parser: '1', '2*(-1+3)', ...."
# define PF(x)  ((parsefun_t)(x))
static const parse_test_t parse_tests[] = {
    { PF(y0parsefile),      NULL,               0, INT_MIN, HUGE_VAL, NULL,    "1.1) enter " Y0_DESC },
    { PF(y0parsefileptr),   stdin,              0, INT_MIN, HUGE_VAL, NULL,    "1.2) enter " Y0_DESC },
    { PF(y1parsefile),      NULL,               0, INT_MAX, -HUGE_VAL, NULL,   "2.1) enter " Y1_DESC },
    { PF(y1parsefile),      NULL,               0, INT_MAX, -HUGE_VAL, NULL,   "2.2) enter " Y1_DESC },
    { PF(y2parsefile),      NULL,               0, INT_MAX, -HUGE_VAL, NULL,   "3.1) enter " Y2_DESC },
    { PF(y2parsefileptr),   stdin,              0, INT_MAX, -HUGE_VAL, NULL,   "3.2) enter " Y2_DESC },
    { PF(y0parsestr),       " 2  *(-1+3  )",    0, 4, HUGE_VAL, NULL,          "1.3) [buffer] " Y0_DESC },
    { PF(y1parsestr),       " cos( 0,0) ",      0, INT_MAX, 1.0, NULL,         "2.3) [buffer] " Y1_DESC },
    { PF(y1parsestr),       " 3.14 ",           0, INT_MAX, 3.14, NULL,        "2.4) [buffer] " Y1_DESC },
    { PF(y2parsestr),       " sin( 0,0) ",      0, INT_MAX, 0.0, NULL,         "3.3) [buffer] " Y2_DESC },
    { PF(y2parsestr),       " 3.14 ",           0, INT_MAX, 3.14, NULL,        "3.4) [buffer] " Y2_DESC },
    { PF(y0parsestr),       " -999 % 1000",     0, -999, HUGE_VAL, NULL,       "1.4) [buffer] " Y0_DESC },
    { PF(y2parsefile),      ",./,./;:",        -1, INT_MIN, HUGE_VAL, NULL,    "3.5) [file_notfound] " Y2_DESC },
#   ifdef BUILD_SYS_linux
    { PF(y0linux_parsestr), " 2  *(-1+3  )",    0, INT_MAX, HUGE_VAL, NULL,    "L.3) [buffer] " Y0L_DESC },
#   endif
    { NULL, NULL, 0, 0, 0.0, NULL, NULL },
};
static bool sisprint(const char * s) {
    while (*s && isprint(*s)) s++;
    return *s == 0;
}
static bool test_parser(const parse_test_t * test, FILE * out, unsigned int * nerrors, unsigned int * nok);
#endif // if BUILD_LEX && BUILD_YACC

extern "C" int m();
extern "C" int cpp_call_for_c(int);
extern "C" int cpp_cni_call_for_c(int);

/** global running state used by signal handler */
static volatile sig_atomic_t s_running = 1;
/** signal handler * /
static void sig_handler(int sig) {
    if (sig == SIGINT)
        s_running = 0;
}
*/

static int usage(int ret, int argc, const char *const* argv) {
    fprintf(ret ? stderr : stdout, "\nUsage: %s [-h] [-s] [-n] [-T]\n"
#           ifdef APP_INCLUDE_SOURCE
            "  -s     : show source\n"
#           endif
            "  -n     : non interactive mode\n"
            "  -T     : test mode\n\n", *argv);
    (void)argc;
    return ret;
}

int main(int argc, const char *const* argv) {
    unsigned int    nerrors = 0, nok = 0;
    int             ret;
    bool            interactive = true;

    fprintf(stdout, "%s v%s %s (built on %s, %s from git:%s) - %s\n\n",
            BUILD_APPNAME, APP_VERSION, BUILD_APPRELEASE, __DATE__, __TIME__, BUILD_GITREV, *argv);

    fprintf(stdout, "Copyright (C) 2018-2019 Vincent Sallaberry.\n"
                    "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n" \
                    "This is free software: you are free to change and redistribute it.\n" \
                    "There is NO WARRANTY, to the extent permitted by law.\n\n");
    fprintf(stdout, "  prefix     : %s\n"
                    "  srcpath    : %s\n"
                    "  git_remote : %s\n"
                    "  git_full   : %s\n"
                    "  CC         : %s\n"
                    "  CXX        : %s\n"
                    "  GCJ        : %s\n"
                    "  CCLD       : %s\n"
                    "  flags      : LEX:%d YACC:%d BISON3:%d JAVAOBJ:%d BIN:%d LIB:%d JAR:%d"
#                   ifdef BUILD_DEBUG
                    " DEBUG"
#                   endif
#                   ifdef BUILD_TEST
                    " TEST"
#                   endif
                    "\n",
                    BUILD_PREFIX, BUILD_SRCPATH, BUILD_GITREMOTE, BUILD_GITREVFULL,
                    BUILD_CC_CMD, BUILD_CXX_CMD, BUILD_GCJ_CMD, BUILD_CCLD_CMD,
                    BUILD_LEX, BUILD_YACC, BUILD_BISON3, BUILD_JAVAOBJ, BUILD_BIN, BUILD_LIB, BUILD_JAR);

    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
            for (const char * arg = argv[i] + 1; *arg; arg++) {
                switch (*arg) {
                    case 'T': break;
                    case 'n': interactive = false; break ;
#                   ifdef APP_INCLUDE_SOURCE
                    case 's':
                        vmultilangdemo_get_source(stdout, NULL, 0, NULL);
                        break ;
#                   endif
                    case 'h': return usage(0, argc, argv);
                    default:  return usage(1, argc, argv);
                }
            }
        } else return usage(2, argc, argv);
    }

    // install SIG_INT handler
    /*struct sigaction sa; sa.sa_handler = sig_handler; sa.sa_flags=SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) < 0) perror("sigaction");*/

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

    // Create argv
    JArray < java::lang::String * > * args =
        (JArray < java::lang::String * > *) JvNewObjectArray(argc, &java::lang::String::class$, NULL);
    for (int i = 0; i < argc; i++) {
        java::lang::String * arg = JvNewStringLatin1(argv[i]);
        elements(args)[i] = arg;
    }
    JMain::main(args);

    // Checking Java Exceptions mecanisms
    // gcj does not support mixing c++ and java exception, then c++ exceptions
    // cannot be caught in this file, and catch (...) is ignored.
    try{
        fprintf(stdout, "+ throwing NullPointerException...\n");
        JMain::main(NULL); // throw new java::lang::NullPointerException();
        nerrors++;
    } catch (java::lang::NullPointerException *e) {
        fprintf(stderr, "+ caught NullPointerException.\n");
        nok++;
    } catch (java::lang::Exception *e) {
        fprintf(stderr, "! caught java::lang::Exception.\n");
        nerrors++;
    }

    // call c/cpp/java cni code
    if ((ret = cpp_cni_call_for_c(0)) != 3) {
        nerrors++;
    } else
        nok++;
    fprintf(stdout, "cpp_cni_call_for_c(0) --> %d\n", ret);

    // call java bison parser
#   if BUILD_BISON3
    Parser * jparser = new Parser(NULL);
    jparser->setDebugLevel(965);
    ret = (int) jparser->getDebugLevel();
    fprintf(stdout, "javaparser addr:%p debuglevel:%d\n", (void*)jparser, ret);
    if (ret == 965) nok++; else nerrors++;
    try {
        //jparser->parse();
    } catch (java::lang::Exception * e) {
        fprintf(stdout, "java exception\n");
        nerrors++;
    }
#   endif // if BUILD_BISON3

    JvDetachCurrentThread();
#  endif // if BUILD_JAVAOBJ

    fprintf(stdout, "\n** running c/cpp/obj sample code for second time\n");
    //m();
    if ((ret = cpp_call_for_c(0)) != 3) {
        nerrors++;
    } else
        nok++;
    fprintf(stdout, "cpp_call_for_c(0) --> %d\n", ret);


    // Run lex/yacc if included in build.
#   if BUILD_LEX && BUILD_YACC
    fprintf(stdout, "\n** Running yacc/lex samples\n");

    for (const parse_test_t * test = parse_tests; s_running && test && test->parsefun; test++) {
        if (interactive || (test->content != NULL && test->content != stdin))
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
    ret = test->parsefun((void*)test->content, &result);
    if (ret != test->yyparse_expected) {
        fprintf(out, "error, <yy>process(): %d, expected %d.\n", ret, test->yyparse_expected);
        (*nerrors)++;
    } else {
        (*nok)++;
    }
    if (ret == 0) {
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
            strncpy(result+sizeof(result)-4, "...", 4);
            fprintf(out, "  = %s", result);
            check = !strcmp(result, test->sexpected);
        } else {
            fprintf(out, "  = ???");
        }
        if (check != CHAR_MIN+4) {
            if (!check) { (*nerrors)++; fputs(" !! [KO]", out); }
            else { (*nok)++; fputs(" [OK]", out); }
        }
        fputc('\n', out);
    }
    return *nerrors == 0;
}
#endif // if BUILD_LEX && BUILD_YACC

