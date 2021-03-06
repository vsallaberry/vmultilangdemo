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
 * Test grammar.
 */
%{
/*
 * CODE Header
 */
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

/* user-defined header for yacc compatibility, preceded by YY prefix (BCOMPAT_YYPREFIX),
 * ('yacc -p<yyprefix>' or '%name-prefix'). */
#define BCOMPAT_YYPREFIX y2
#define BCOMPAT_LEX_CXX
#include "bison_compat.h"

%}

/*
 * Parser parameters
 */
/* yacc/bison versions have different way to change yy prefix
 * the common way to do it is to use the command line option '-p<yyprefix>'
 * yacc is cool, bison is very, very cool */
/* %name-prefix = "y1" - file built with 'yacc -p<yyprefix>' */
/* %define "api.prefix" "y1" - file built with 'yacc -p<yyprefix>' */
/* %error-verbose - not supported on all yacc */
/* %locations - not supported on all yacc */
/* %parse-param { long *presult } */
/* %lex-param, %param (lex&parse) */
/* %define api.pure full - not supported on all yacc */

/* Tokens definitions */
%token NUMBER
/*%token <number> NUMBER "number" -- bsd yacc does not like it */
%token SIN
%token TAN
%token LPAREN   "("
%token RPAREN   ")"

%type <number>  exp
%type <number>  NUMBER

%union {
    double number;
}

/* Grammar */
%%
program :
exp                         { if (my_yyresult) *((double*)my_yyresult) = $1; }
;

exp :
NUMBER                      { $$ = $1; }
| TAN LPAREN exp RPAREN     { $$ = tan($3); }
| SIN LPAREN exp RPAREN     { $$ = sin($3); }
| LPAREN exp RPAREN         { $$ = $2; }
| error                     { my_yyresult=NULL; BCOMPAT_YYABORT; }
;

%%
/*
 * CODE footer
 */
/* nothing: <yyprefix>error, v_<yyprefix>error and wrap_<yyprefix>parse defined to default ones in bison_compat.h */

