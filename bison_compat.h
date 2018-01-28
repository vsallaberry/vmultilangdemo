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
 * Header for lex-flex/yacc-bison compatibility.
 *
 * This adds support for '%locations', '%error-verbose', '%parse-param'
 * not supported by all yacc. For '%error-verbose', verbose done if yacc supports it.
 *
 * - if the bison/yacc generated header is included in the same file as this file,
 *   this file MUST always be included AFTER bison/yacc generated header.
 *
 * - At head of .y* file, include this file, preceded by:
 *   + '#define BCOMPAT_YYPREFIX <yyprefix>'
 *   + '#define BCOMPAT_NO_VYYERROR if you don't want to use the default
 *      yyerror(const char*) and vyyerror(const char *, ...) methods.
 *   + ''
 *
 * - At head of .l* file, include this file, with the same prerequites as for
 *   .y* file.
 *
 * - in the user file calling the parser, include this file and use
 *   'BCOMPAT_PARSER_DECL(prefix);' to declare the <prefix>parsefile(char*,void*)
 *   and <prefix>parsestr(char*,void*) calling scanner, parser and setting the result.
 *
 * - To summarize, including this header in .l* and .y* files adds the following:
 *   + int v_<prefix>error(const char*, ...) and int <prefix>error(const char*)
 *     -> for error print with locations from .y* and .l*.
 *   + int <prefix>parsefile(const char*,void*), <prefix>parsestr(const char*,void*),
 *     for high level (from outside) parser call
 *   + BCOMPAT_PARSER_DECL(prefix)
 *
 * - pure parser (reentrant) is not supported.
 *
 */
#ifndef BISON_COMPAT_H
# define BISON_COMPAT_H

# if defined(FLEX_SCANNER)
#  define BCOMPAT_IS_LEX
#  undef BCOMPAT_IS_YACC
# elif defined(YYBISON) || defined(YYBYYACC) || defined(YYEMPTY)
#   define BCOMPAT_IS_YACC
#   undef BCOMPAT_IS_LEX
# endif /* ifdef LEX,YACC, OTHER */

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
#   pragma message "** generated YACC or LEX file"
#  endif

#  ifndef BCOMPAT_YYPREFIX
#   define BCOMPAT_YYPREFIX yy
#  endif
#  define BCOMPAT_YYLOCATIONS
#  define BCOMPAT_YYPARSE_PARAM

#  define BCOMPAT_ADD_PREFIX(p,s) p##s
#  define BCOMPAT_EXPAND(p,s) BCOMPAT_ADD_PREFIX(p,s)

#  ifdef BCOMPAT_YYLOCATIONS
typedef struct {
    unsigned int    first_line;
    unsigned int    first_column;
    unsigned int    last_line;
    unsigned int    last_column;
    const char *    filename;
} my_yylloc_t;
#  endif /* ifdef BCOMPAT_YYLOCATIONS */

#  ifndef BCOMPAT_NO_VYYERROR
/* create <prefix>error() and v_<prefix>error() */
#   define BCOMPAT_YYERROR_DEFAULT   BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, error)
#   define BCOMPAT_V_YYERROR_DEFAULT BCOMPAT_EXPAND(v_, BCOMPAT_YYERROR_DEFAULT)
#   define BCOMPAT_DECL_V_YYERROR   int BCOMPAT_V_YYERROR_DEFAULT(const char * msg, ...)
BCOMPAT_DECL_V_YYERROR;
#  endif /* ifndef BCOMPAT_NO_VYYERROR */

/* declare internal wrap_<prefix>parse(), <prefix>lex(),<prefix>error,<prefix>parse */
#  define BCOMPAT_YYPARSE_WRAPPER       BCOMPAT_EXPAND(wrap_, BCOMPAT_EXPAND(BCOMPAT_YYPREFIX,parse))
#  define BCOMPAT_DECL_YYPARSE_WRAPPER  int BCOMPAT_YYPARSE_WRAPPER(my_yylloc_t *p_yylloc, void * p_yyresult)
#  define BCOMPAT_YYPARSE               BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, parse)
#  define BCOMPAT_YYLEX                 BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, lex)
#  define BCOMPAT_YYERROR               BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, error)
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
#   pragma message "** generated YACC file"
#  endif
#  ifndef YYERROR_VERBOSE
#   define YYERROR_VERBOSE 1
#  endif
#  include <stdarg.h>

/* code to be used to stop parsing and quit yyparse() with an error */
#  define BCOMPAT_YYABORT   YYABORT

/* user-defined yy location structure (%locations not supported by all yacc) */
static my_yylloc_t *    my_yylloc;
/* user-defined yyparse output ('%parse-param {long *res}' not supported by all yacc) */
static void *           my_yyresult;

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
            fprintf(out, "l%d.%d-%d: ", my_yylloc->first_line,
                    my_yylloc->first_column, my_yylloc->last_column-1);
        } else {
            fprintf(out, "l%d.%d-l%d.%d: ",
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
    my_yylloc = p_yylloc;
    my_yyresult = p_yyresult;
    return BCOMPAT_YYPARSE();
}

# elif defined(BCOMPAT_IS_LEX)
/********************************************************
 * DECLARATIONS for LEX GENERATED SOURCE.
 ********************************************************/
#  if defined(_DEBUG) && defined(_TEST)
#   pragma message "** generated LEX file"
#  endif

#  define BCOMPAT_YYPARSEALL    BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, parseall)
#  define BCOMPAT_YYPARSEFILE   BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, parsefile)
#  define BCOMPAT_YYPARSESTR    BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, parsestr)
#  define BCOMPAT_YYSCANSTRING  BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, _scan_string)
#  define BCOMPAT_YYIN          BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, in)
#  define BCOMPAT_YYLLENG       BCOMPAT_EXPAND(BCOMPAT_YYPREFIX, leng)

/* yylloc location structure */
//static my_yylloc_t my_yylloc = { .first_column = INT_MAX, .first_line = 0, .last_column=0, .last_line=0 };
static my_yylloc_t my_yylloc = { INT_MAX, INT_MAX, INT_MAX, INT_MAX, NULL };

/* Code run each time a pattern is matched. */
#  ifdef BCOMPAT_YYLOCATIONS
#   define YY_USER_ACTION  do { \
        my_yylloc.first_column = my_yylloc.last_column; \
        my_yylloc.last_column+=BCOMPAT_YYLLENG; \
    } while(0);
#  endif

static int BCOMPAT_YYPARSEALL(const char *str, const char *filename, void * presult) {
    int yyresult;

    if (my_yylloc.first_column != INT_MAX) {
        fprintf(stderr, "Scan error, %s() is not reentrant\n", __func__);
        return -1;
    }
    if (str == NULL) {
        BCOMPAT_YYIN = filename ? fopen(filename, "r") : stdin;
        if (!BCOMPAT_YYIN) {
            fprintf(stderr, "Scan error, cannot open file '%s': %s\n", filename, strerror (errno));
            return -1;
        }
    } else {
        BCOMPAT_YYIN = NULL;
        BCOMPAT_YYSCANSTRING(str);
    }
    my_yylloc.first_line = my_yylloc.first_column = my_yylloc.last_line = my_yylloc.last_column = 1;
    yyresult = BCOMPAT_YYPARSE_WRAPPER(&my_yylloc, presult);
    if (str == NULL && BCOMPAT_YYIN != stdin) {
        fclose(BCOMPAT_YYIN);
    }
    my_yylloc.first_column = INT_MAX;
    return yyresult;
}

BCOMPAT_EXPORT int BCOMPAT_YYPARSEFILE(const char *filename, void * presult) {
    return BCOMPAT_YYPARSEALL(NULL, filename, presult);
}

BCOMPAT_EXPORT int BCOMPAT_YYPARSESTR(const char *str, void * presult) {
    if (!str) {
        fprintf(stderr, "Scan error: string is null\n");
        return -1;
    }
    return BCOMPAT_YYPARSEALL(str, NULL, presult);
}

# else
/************************************************************
 * DECLARATIONS FOR OTHER FILES THAN LEX/YACC GENERATED ONES.
 ************************************************************/
#  if defined(_DEBUG) && defined(_TEST)
#   pragma message "** OTHER FILE"
#  endif

#  define BCOMPAT_PARSER_DECL(prefix)   BCOMPAT_EXPORT int prefix##parsefile(const char *filename, void * presult); \
                                        BCOMPAT_EXPORT int prefix##parsestr(const char *str, void * presult)
//#  define BCOMPAT_PARSESTR_DECL(prefix)     BCOMPAT_EXPORT int prefix##parsestr(const char *str, void * presult)

/********************************************************
 * END OF DECLARATIONS.
 ********************************************************/
# endif /* ifdef BCOMPAT_IS_YACC,BCOMPAT_IS_LEX,else */
#undef BCOMPAT_IS_LEX
#undef BCOMPAT_IS_YACC
#endif /* ifndef BISON_COMPAT_H */

