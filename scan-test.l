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
%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
//#include <stdint.h>
#include <limits.h>

/* include bison generated header */
#include "parse-test.h"

/* user-defined header for yacc compatibility, preceded by YY prefix (%option prefix=). */
#define BCOMPAT_YYPREFIX y0
#include "bison_compat.h"

%}

%option noyywrap
%option nounput
%option noinput
%option prefix="y0"
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
        n = strtol(y0text, NULL, 10);
        if (n < INT_MIN || n > INT_MAX || errno == ERANGE) {
            v_y0error("scan error, integer out of range.");
            yyterminate();
        } else {
            y0lval.number = n;
            return NUMBER;
        }
    }
    {space}     ; /* nothing. could be used to update location */
    {line}      yyterminate();
    <<EOF>>     yyterminate();
    . {
        v_y0error("scan error, unexpected %c", y0text[0]);
        return *y0text;
	}
}

%%
/* nothing: y0parsestring and y0parsestr defined to default ones in bison_compat.h */

