%{
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
 * Test lexer.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

/* include bison generated header */
#include "parse-test.h"

/* user-defined header for yacc compatibility, preceded by YY prefix (BCOMPAT_YYPREFIX),
 * ('lex -P<yyprefix>' or '%option prefix=<yyprefix>'). */
#define BCOMPAT_YYPREFIX y0linux_
#include "bison_compat.h"

%}

%option noyywrap
%option nounput
%option noinput
/* %option prefix="y0linux_" - file built with 'lex -P<yyprefix> */
/* %parse-param {YYSTYPE *lvalp} {YYLTYPE *llocp} - not supported on all lex */
/* %option batch - can look for token in advance. Disable for interactive mode */
/* %option noyy_top_state */

number  [0-9]+
space	[ \t]+
line    [\n\r]

%%

<INITIAL>
{
    "+"         return PLUS;
    "-"         return MINUS;
    "*"         return TIMES;
    "/"         return DIVIDE;
    "%"         return MOD;
    "("         return LPAREN;
    ")"         return RPAREN;
    {number} {
        long n;
        errno = 0;
        n = strtol(y0linux_text, NULL, 10);
        if (errno != 0) {
            /* v_y0linux_error("scan error, integer out of range."); */
            fprintf(stderr, "scan error, integer out of range.\n");
            yyterminate();
        } else {
            /* y0linux_lval.number = n; */
            return NUMBER;
        }
    }
    {space}     ; /* nothing. could be used to update location */
    {line}      yyterminate();
    <<EOF>>     yyterminate();
    . {
        /* v_y0linux_error("scan error, unexpected %c", y0linux_text[0]); */
        fprintf(stderr, "scan error, unexpected %c\n", y0linux_text[0]);
        return *y0linux_text;
	}
}

%%
/* define wrap_<yyprefix>parse() because this scanner does not go with a parser */
BCOMPAT_DECL_YYPARSE_WRAPPER {
    (void)p_yyresult;
    (void)p_yylloc;
    (void)p_yylexer;
    int c;
    while ((c = y0linux_lex()) != YY_NULL) {
        fprintf(stderr, "tok %d <%s>\n", c, y0linux_text);
    }
    return 0;
}

/* nothing: <yyprefix>parse{str,file,buffer,fileptr} defined to default ones in bison_compat.h */

