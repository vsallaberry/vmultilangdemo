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
 * Test lexer.
 */
%{
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <climits>
#include <cmath>

/* include bison generated header */
#include "parse-test2.hh"

/* user-defined header for yacc compatibility, preceded by YY prefix (BCOMPAT_YYPREFIX),
 * ('lex -P<yyprefix>' or '%option prefix=<yyprefix>'). */
#define BCOMPAT_YYPREFIX y1
#include "bison_compat.h"

%}

%option noyywrap
%option nounput
%option noinput
/* %option prefix="y1" - file built with 'lex -P<yyprefix> */
/* %parse-param {YYSTYPE *lvalp} {YYLTYPE *llocp} - not supported on all lex */
/* %option batch - can look for token in advance. Disable for interactive mode */
/* %option noyy_top_state */

number  [0-9]+([.,][0-9]*)?
space	[ \t]+
line    [\n\r]

%%

<INITIAL>
{
    "cos"       return COS;
    "("         return LPAREN;
    ")"         return RPAREN;
    {number} {
        char * endptr = NULL;
        double n;
        errno = 0;
        n = strtod(y1text, &endptr);
        if (endptr && *endptr && (*endptr == '.' || *endptr == ',')) {
            *endptr = *endptr == '.' ? ',' : '.';
            errno = 0;
            n = strtod(y1text, &endptr);
            *endptr = *endptr == '.' ? ',' : '.';
        }
        if (errno != 0) {
            v_y1error("scan error, float out of range.");
            yyterminate();
        } else {
            y1lval.number = n;
            return NUMBER;
        }
    }
    {space}     ; /* nothing. could be used to update location */
    {line}      yyterminate();
    <<EOF>>     yyterminate();
    . {
        v_y1error("scan error, unexpected %c", y1text[0]);
        return *y1text;
	}
}

%%
/* nothing: <yyprefix>parse{str,file,buffer,fileptr} defined to default ones in bison_compat.h */

