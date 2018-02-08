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
 * Header for lex-flex/yacc-bison compatibility: yacc is cool, bison is very, very cool.
 *
 * This adds support for '%locations', '%error-verbose', '%parse-param' not supported
 * by all yacc. For '%error-verbose', verbose done if yacc supports YY_VERBOSE=1, for
 * '%locations' they are only based on scanner (current token).
 *
 * - if the bison/yacc generated header is included in the same file as this file,
 *   this file MUST always be included AFTER bison/yacc generated header.
 *
 * - At head of .y* file, (built with 'yacc -p<yyprefix>'),include this file, preceded by:
 *   + '#define BCOMPAT_YYPREFIX <yyprefix>'
 *   + '#define BCOMPAT_LEX_CXX' if you generate scanner with '%option c++'
 *   + [optional] '#define BCOMPAT_NO_VYYERROR' -> don't create <yyprefix>error(const char*)
 *     and v_<yyprefix>error(const char*, ...)
 *   + [optional] '#define BCOMPAT_NO_YYLOCATIONS' -> don't use LOCATIONS.
 *   + [optional] '#define BCOMPAT_IS_{YACC,LEX,OTHER}' to force type of file including this one.
 *
 * - At head of .l* file, include this file, with the same prerequites as for .y* file.
 *
 * - in user file calling the parser, include this file and use 'BCOMPAT_PARSER_DECL(prefix);'
 *   to declare the <prefix>parsefile(char*,void*), <prefix>parsestr(char*,void*) and
 *   <prefix>parsefileptr(FILE*,void*) calling scanner, parser and setting the result.
 *
 * - To summarize, including this header in .l* and .y* files adds the following:
 *   + int v_<yyprefix>error(const char*, ...) and int <yyprefix>error(const char*)
 *     -> for error print with locations from .y* and .l*.
 *   + 'BCOMPAT_PARSER_DECL(yyprefix);', exporting int <yyprefix>parsefile(const char*,void*),
 *     int <yyprefix>parsestr(const char*,void*), int <yyprefix>parsefileptr(FILE*,void*),
 *     int <yyprefix>parsebuffer(char*,size_t,void*) for parser call from outside.
 *
 * - pure parser (reentrant) is not supported.
 *
 */
#ifndef BISON_COMPAT_H
# define BISON_COMPAT_H

/*
 * Guess from where this file has been included: yacc-generated,
 * flex-generated, or other.
 * You can force it by defining BCOMPAT_IS_LEX, BCOMPAT_IS_YACC
 * or BCOMPAT_IS_OTHER, before including this file.
 */
# if ! defined(BCOMPAT_IS_YACC) && ! defined(BCOMPAT_IS_LEX) && ! defined(BCOMPAT_IS_OTHER)
#  if defined(FLEX_SCANNER)
#   define BCOMPAT_IS_LEX
#   undef BCOMPAT_IS_YACC
#   undef BCOMPAT_IS_OTHER
#  elif defined(YYBISON) || defined(YYBYYACC) || defined(YYEMPTY)
#   define BCOMPAT_IS_YACC
#   undef BCOMPAT_IS_LEX
#   undef BCOMPAT_IS_OTHER
#  else
#   define BCOMPAT_IS_OTHER
#   undef BCOMPAT_IS_YACC
#   undef BCOMPAT_IS_LEX
#  endif /* ifdef LEX,YACC, OTHER */
# endif /* ! (defined(is_yacc|is_lex|is_other) */

/* exported symbols are always in "C" notation */
#  ifdef __cplusplus
#   define BCOMPAT_EXPORT extern "C"
#  else
#   define BCOMPAT_EXPORT
#  endif /* ifdef __cplusplus */

# if defined(BCOMPAT_IS_YACC) || defined(BCOMPAT_IS_LEX)
/********************************************************
 * DECLARATIONS for YACC OR LEX GENERATED SOURCE.
 ********************************************************/
#  if defined(_DEBUG) && defined(_TEST)
/* #   pragma message "** generated YACC or LEX file" */
#  endif

#  ifdef __cplusplus
#   include <cstdarg>
#   include <cstdio>
#   include <cstdlib>
#   include <climits>
#  else
#   include <stdarg.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <limits.h>
#  endif

#  ifndef BCOMPAT_YYPREFIX
#   define BCOMPAT_YYPREFIX yy
#  endif
#  ifndef BCOMPAT_NO_YYLOCATIONS
#   define BCOMPAT_YYLOCATIONS
#  endif
#  define BCOMPAT_YYPARSE_PARAM

/* if Lex is in C++ mode, only the class has a changed prefix */
#  ifdef BCOMPAT_LEX_CXX
#   define BCOMPAT_LEX_YYPREFIX yy
#   undef yylex
#  else
#   define BCOMPAT_LEX_YYPREFIX BCOMPAT_YYPREFIX
//#define yyparse(___void_unsused___) _##y45parse(bcompat_yylloc_t *p_yylloc, void * p_yyresult, FlexLexer * p_lexer)
//#define yyparse(void) _##y45parse(void *p)
#  endif

#  define BCOMPAT_ADD_PREFIX(p,s) p##s
#  define BCOMPAT_EXPAND(p,s) BCOMPAT_ADD_PREFIX(p,s)

typedef struct bcompat_yylloc_s bcompat_yylloc_t;
#  ifdef BCOMPAT_YYLOCATIONS
struct bcompat_yylloc_s {
    unsigned int    first_line;
    unsigned int    first_column;
    unsigned int    last_line;
    unsigned int    last_column;
    char *          filename;
};
#  endif /* ifdef BCOMPAT_YYLOCATIONS */

#  ifndef BCOMPAT_NO_VYYERROR
/* create <yyprefix>error() and v_<yyprefix>error() */
#   define BCOMPAT_YYERROR_DEFAULT   BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, error)
#   define BCOMPAT_V_YYERROR_DEFAULT BCOMPAT_EXPAND(v_, BCOMPAT_YYERROR_DEFAULT)
#   define BCOMPAT_DECL_V_YYERROR   int BCOMPAT_V_YYERROR_DEFAULT(const char * msg, ...)
BCOMPAT_DECL_V_YYERROR;
#  endif /* ifndef BCOMPAT_NO_VYYERROR */

/* declare internal wrap_<yyprefix>parse(), <yyprefix>{lex(),parse(),error(),out} */
#  define BCOMPAT_YYPARSE_WRAPPER       BCOMPAT_EXPAND(wrap_, BCOMPAT_EXPAND(BCOMPAT_YYPREFIX,parse))
#  define BCOMPAT_DECL_YYPARSE_WRAPPER  int BCOMPAT_YYPARSE_WRAPPER(bcompat_yylloc_t *p_yylloc, void * p_yylexer, void * p_yyresult)
#  define BCOMPAT_YYPARSE               BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, parse)
#  define BCOMPAT_YYERROR               BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, error)
#  define BCOMPAT_YYLEX                 BCOMPAT_EXPAND(BCOMPAT_LEX_YYPREFIX, lex)
int BCOMPAT_YYLEX();
int BCOMPAT_YYERROR(const char *msg);
int BCOMPAT_YYPARSE();
BCOMPAT_DECL_YYPARSE_WRAPPER;

# endif /* ifdef BCOMPAT_IS_LEX || BCOMPAT_IS_YACC */

# if defined(BCOMPAT_IS_YACC)
/********************************************************
 * DECLARATIONS for YACC GENERATED SOURCE.
 ********************************************************/
#  if defined(_DEBUG) && defined(_TEST)
/* #   pragma message "** generated YACC file" */
#  endif
#  ifndef YYERROR_VERBOSE
#   define YYERROR_VERBOSE 1
#  endif

#  ifdef BCOMPAT_LEX_CXX
#   include <FlexLexer.h>
#   undef yylex
#   define yylex() (((FlexLexer*)my_yylexer)->yylex())
#  endif
static void * my_yylexer = NULL;

/* code to be used to stop parsing and quit yyparse() with an error */
#  define BCOMPAT_YYABORT   YYABORT

/* user-defined yy location structure (%locations not supported by all yacc) */
static bcompat_yylloc_t *   my_yylloc = NULL;
/* user-defined yyparse output ('%parse-param {long *res}' not supported by all yacc) */
static void *               my_yyresult = NULL;

/* defining default yyerror unless user has set BCOMPAT_NO_VYYERROR */
#  ifndef BCOMPAT_NO_VYYERROR
BCOMPAT_DECL_V_YYERROR {
    FILE *  out = stderr;
    va_list args;

    va_start(args, msg);
    flockfile(out);
#  ifdef BCOMPAT_YYLOCATIONS
    if (my_yylloc) {
        if (my_yylloc->first_line == my_yylloc->last_line) {
            fprintf(out, "%s: l%d.%d-%d: ", my_yylloc->filename, my_yylloc->first_line,
                    my_yylloc->first_column, my_yylloc->last_column-1);
        } else {
            fprintf(out, "%s: l%d.%d-l%d.%d: ", my_yylloc->filename,
                    my_yylloc->first_line, my_yylloc->first_column,
                    my_yylloc->last_line, my_yylloc->last_column-1);
        }
    }
#endif /* ifdef BCOMPAT_YYLOCATIONS */
    vfprintf(out, msg, args);
    fputc('\n', out);
    funlockfile(out);
    va_end(args);
    return 1;
}
int	BCOMPAT_YYERROR_DEFAULT(const char *msg) {
    return BCOMPAT_V_YYERROR_DEFAULT(msg);
}
#  endif /* ifndef BCOMPAT_NO_VYYERROR */

/* yyparse wrapper */
BCOMPAT_DECL_YYPARSE_WRAPPER {
    my_yylexer = p_yylexer;
    my_yylloc = p_yylloc;
    my_yyresult = p_yyresult;
    return BCOMPAT_YYPARSE();
}

# elif defined(BCOMPAT_IS_LEX)
/********************************************************
 * DECLARATIONS for LEX GENERATED SOURCE.
 ********************************************************/
#  if defined(_DEBUG) && defined(_TEST)
/* #   pragma message "** generated LEX file" */
#  endif

#  define BCOMPAT_YYPARSEALL        BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, parseall)
#  define BCOMPAT_YYPARSEFILE       BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, parsefile)
#  define BCOMPAT_YYPARSESTR        BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, parsestr)
#  define BCOMPAT_YYPARSEFILEPTR    BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, parsefileptr)
#  define BCOMPAT_YYPARSEBUFFER     BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, parsebuffer)
#  define BCOMPAT_YYPARSEDESTROY    BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, parsedestroy)
#  define BCOMPAT_YYSCANSTRING      BCOMPAT_EXPAND(BCOMPAT_LEX_YYPREFIX, _scan_string)
#  define BCOMPAT_YYSCANBUFFER      BCOMPAT_EXPAND(BCOMPAT_LEX_YYPREFIX, _scan_buffer)
#  define BCOMPAT_YYIN              BCOMPAT_EXPAND(BCOMPAT_LEX_YYPREFIX, in)
#  define BCOMPAT_YYOUT             BCOMPAT_EXPAND(BCOMPAT_LEX_YYPREFIX, out)
#  define BCOMPAT_YYLLENG           BCOMPAT_EXPAND(BCOMPAT_LEX_YYPREFIX, leng)
#  define BCOMPAT_YYLEXDESTROY      BCOMPAT_EXPAND(BCOMPAT_LEX_YYPREFIX, lex_destroy)
#  define BCOMPAT_YYFLEXLEXER       BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, FlexLexer)
int BCOMPAT_YYLEXDESTROY();

#  ifdef BCOMPAT_YYLOCATIONS
/* yylloc location structure */
static bcompat_yylloc_t my_yylloc = { INT_MAX, INT_MAX, INT_MAX, INT_MAX, NULL };
/* Code run each time a pattern is matched. */
#   define YY_USER_ACTION  do { \
        my_yylloc.first_column = my_yylloc.last_column; \
        my_yylloc.last_column+=BCOMPAT_YYLLENG; \
    } while(0);
#  endif

#  ifdef BCOMPAT_LEX_CXX
#   include <streambuf>
#   include <sstream>
#   include <fstream>
#   include <string>
#   if defined(__GLIBCXX__) || defined(_GLIBCXX_BEGIN_NAMESPACE_VERSION) || defined(_GLIBCXX_BEGIN_NAMESPACE)
#    define BCOMPAT_STDIO_FILEBUF
#    include <ext/stdio_filebuf.h>
#   else
#    pragma message "no __gnucxx::stdio_filebuf --> no <yyprefix>parsefileptr()"
#   endif // ifdef GLIBCXX...
static int BCOMPAT_YYPARSEALL(const char *str, size_t size, const char *filename, FILE *pfile, void * presult) {
    FlexLexer *                 lexer = NULL;
    std::basic_streambuf<char>* streambuf = NULL;
    std::string *               string = NULL;
    std::istream *              istream = NULL;
    bcompat_yylloc_t *          p_yylloc = NULL;
    int                         yyresult;
#   ifdef BCOMPAT_YYLOCATIONS
    //bcompat_yylloc_t          yylloc = { 1, 1, 1, 1, NULL };
    const char *                input_name = filename;

    //p_yylloc = &yylloc;
    p_yylloc = &my_yylloc;
    if (my_yylloc.first_column != INT_MAX) {
        fprintf(stderr, "Scan error, %s() is not reentrant\n", __func__);
        return -1;
    }
#   endif
    if (str != NULL) {
        if (size > 0) {
            string = new std::string(str, size);
        } else {
            string = new std::string(str);
        }
        istream = new std::istringstream(*string);
    } else if (pfile != NULL) {
#       ifdef BCOMPAT_STDIO_FILEBUF
        streambuf = new __gnu_cxx::stdio_filebuf<char>(pfile, std::ios::in);
        istream = new std::istream(streambuf);
#       else
        fprintf(stderr, "Scan error, <yyprefix>parsefileptr() not supported on this system, need __gnu_cxx:stdio_filebuf.\n");
        return -1;
#       endif
    } else {
        if (filename == NULL) {
            istream = &std::cin;
        } else {
            std::filebuf * filebuf = new std::filebuf();
            streambuf = filebuf;
            if (!(filebuf->open(filename, std::ios::in))) {
                delete filebuf;
                fprintf(stderr, "Scan error, cannot open file '%s': %s\n", filename, strerror (errno));
                return -1;
            }
            istream = new std::istream(filebuf);
        }
    }

    lexer = new BCOMPAT_YYFLEXLEXER(istream, &std::cerr);

#   ifdef BCOMPAT_YYLOCATIONS
    if (istream == &std::cin) input_name = "<stdin>";
    p_yylloc->filename = strdup(input_name ? input_name : "<?>");
    p_yylloc->first_line = p_yylloc->first_column = p_yylloc->last_line = p_yylloc->last_column = 1;
#   endif
    yyresult = BCOMPAT_YYPARSE_WRAPPER(p_yylloc, lexer, presult);
    if (pfile != NULL && pfile != stdin) {
        fclose(pfile);
    }
#   ifdef BCOMPAT_YYLOCATIONS
    if (p_yylloc->filename) {
        free(p_yylloc->filename);
        p_yylloc->filename = NULL;
    }
    p_yylloc->first_column = INT_MAX;
#   endif
    if (lexer)
        delete lexer;
    if (istream && istream != &std::cin)
        delete istream;
    if (streambuf) {
        //streambuf->close();
        delete streambuf;
    }
    if (string)
        delete string;
    return yyresult;
}
#  else
static int BCOMPAT_YYPARSEALL(const char *str, size_t size, const char *filename, FILE *pfile, void * presult) {
    bcompat_yylloc_t *  p_yylloc = NULL;
    int                 yyresult;
#   ifdef BCOMPAT_YYLOCATIONS
    //bcompat_yylloc_t    yylloc = { 1, 1, 1, 1, NULL };
    const char *        input_name = filename;

    //p_yylloc = &yylloc;
    p_yylloc = &my_yylloc;
    if (p_yylloc->first_column != INT_MAX) {
        fprintf(stderr, "Scan error, %s() is not reentrant\n", __func__);
        return -1;
    }
#   endif
    if (str != NULL) {
        BCOMPAT_YYIN = NULL;
        if (size > 0) {
            BCOMPAT_YYSCANBUFFER((char*)str, size);
        } else {
            BCOMPAT_YYSCANSTRING(str);
        }
    } else if (pfile != NULL) {
        BCOMPAT_YYIN = pfile;
    } else {
        BCOMPAT_YYIN = filename ? fopen(filename, "r") : stdin;
        if (!(BCOMPAT_YYIN)) {
            fprintf(stderr, "Scan error, cannot open file '%s': %s\n", filename, strerror (errno));
            return -1;
        }
    }
#   ifdef BCOMPAT_YYLOCATIONS
    if ((BCOMPAT_YYIN) == stdin) input_name = "<stdin>";
    p_yylloc->filename = strdup(input_name ? input_name : "<?>");
    p_yylloc->first_line = p_yylloc->first_column = p_yylloc->last_line = p_yylloc->last_column = 2;
#   endif
    yyresult = BCOMPAT_YYPARSE_WRAPPER(p_yylloc, NULL, presult);
    if ((BCOMPAT_YYIN) != NULL && (BCOMPAT_YYIN) != stdin) {
        fclose(BCOMPAT_YYIN);
    }
    BCOMPAT_YYLEXDESTROY();
#   ifdef BCOMPAT_YYLOCATIONS
    if (p_yylloc->filename) {
        free(p_yylloc->filename);
        p_yylloc->filename = NULL;
    }
    p_yylloc->first_column = INT_MAX;
#   endif
    return yyresult;
}
#  endif

BCOMPAT_EXPORT int BCOMPAT_YYPARSEFILE(const char *filename, void * presult) {
    return BCOMPAT_YYPARSEALL(NULL, 0, filename, NULL, presult);
}

BCOMPAT_EXPORT int BCOMPAT_YYPARSEFILEPTR(FILE * pfile, void * presult) {
    if (!pfile) {
        fprintf(stderr, "Scan error: file_ptr is null\n");
        return -1;
    }
    return BCOMPAT_YYPARSEALL(NULL, 0, "<file>", pfile, presult);
}

BCOMPAT_EXPORT int BCOMPAT_YYPARSESTR(const char *str, void * presult) {
    if (!str) {
        fprintf(stderr, "Scan error: string is null\n");
        return -1;
    }
    return BCOMPAT_YYPARSEALL(str, 0, "<str>", NULL, presult);
}

BCOMPAT_EXPORT int BCOMPAT_YYPARSEBUFFER(char *base, size_t size, void * presult) {
    if (!base) {
        fprintf(stderr, "Scan error: buffer is null\n");
        return -1;
    }
    return BCOMPAT_YYPARSEALL(base, size, "<buffer>", NULL, presult);
}

BCOMPAT_EXPORT int BCOMPAT_YYPARSEDESTROY() {
#ifndef BCOMPAT_LEX_CXX
    BCOMPAT_YYLEXDESTROY();
#endif
    return 0;
}

# else
/************************************************************
 * DECLARATIONS FOR OTHER FILES THAN LEX/YACC GENERATED ONES.
 ************************************************************/
#  if defined(_DEBUG) && defined(_TEST)
/* #   pragma message "** OTHER FILE" */
#  endif

#  ifdef __cplusplus
#   include <cstdio>
#   include <cstdlib>
#  else
#   include <stdio.h>
#   include <stdlib.h>
#  endif

#  define BCOMPAT_PARSER_DECL(prefix)   BCOMPAT_EXPORT int prefix##parsefile(const char * filename, void * presult); \
                                        BCOMPAT_EXPORT int prefix##parsestr(const char * str, void * presult); \
                                        BCOMPAT_EXPORT int prefix##parsefileptr(FILE * pfile, void * presult); \
                                        BCOMPAT_EXPORT int prefix##parsebuffer(char * base, size_t size); \
                                        BCOMPAT_EXPORT int prefix##parsedestroy()

/********************************************************
 * END OF DECLARATIONS.
 ********************************************************/
# endif /* ifdef BCOMPAT_IS_YACC,BCOMPAT_IS_LEX,else */
#endif /* ifndef BISON_COMPAT_H */

