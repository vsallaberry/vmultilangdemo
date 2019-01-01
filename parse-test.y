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
 *
 * It uses bison_compat.h to simulate '%locations', '%error-verbose',
 * '%parse-param', not supported by all yacc.
 *
 */
%{
/*
 * CODE Header
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* user-defined header for yacc compatibility, preceded by YY prefix (BCOMPAT_YYPREFIX),
 * ('yacc -p<yyprefix>' or '%name-prefix'). */
#define BCOMPAT_YYPREFIX y0
#include "bison_compat.h"

/* divide_by_0 check shortcut to be used in grammar rules */
#define CHECK_DIVIDE_0(n)       if ((n) == 0) { \
                                    my_yyresult = NULL; \
                                    v_y0error("error: bad idea to divide by 0"); \
                                    BCOMPAT_YYABORT; \
                                } else

%}

/*
 * Parser parameters
 */
/* yacc/bison versions have different way to change yy prefix
 * the common way to do it is to use the command line option '-p<yyprefix>'
 * yacc is cool, bison is very, very cool */
/* %name-prefix = "y0" - file built with 'yacc -p<yyprefix>' */
/* %define "api.prefix" "y0" - file built with 'yacc -p<yyprefix>' */
/* %error-verbose - not supported on all yacc */
/* %locations - not supported on all yacc */
/* %parse-param { long *presult } */
/* %lex-param, %param (lex&parse) */
/* %define api.pure full - not supported on all yacc */

/* Tokens definitions */
%token NUMBER
/*%token <number> NUMBER "number" - bsd yacc does not like it */
%token PLUS     "+"
%token MINUS    "-"
%token TIMES    "*"
%token DIVIDE   "/"
%token MOD      "%"
%token LPAREN   "("
%token RPAREN   ")"

%type <number>  exp
%type <number>  NUMBER

%union {
    long number;
}

/* Priority definitions */
%left			MINUS PLUS
%left			DIVIDE TIMES MOD
%nonassoc		UMINUS

/* Grammar */
%%
program :
exp                         { if (my_yyresult) *((long*)my_yyresult) = $1; }
;

exp :
NUMBER                      { $$ = $1; }
| MINUS exp %prec UMINUS    { $$ = -$2; }
| exp PLUS exp              { $$ = $1 + $3; }
| exp MINUS exp             { $$ = $1 - $3; }
| exp TIMES exp             { $$ = $1 * $3; }
| exp DIVIDE exp            { CHECK_DIVIDE_0($3) { $$ = $1 / $3; } }
| exp MOD exp               { CHECK_DIVIDE_0($3) $$ = $1 % $3; }
| LPAREN exp RPAREN         { $$ = $2; }
| error                     { my_yyresult = NULL; BCOMPAT_YYABORT; }
;

%%
/*
 * CODE footer
 */
/* nothing: <yyprefix>error, v_<yyprefix>error and wrap_<yyprefix>parse defined to default ones in bison_compat.h */

