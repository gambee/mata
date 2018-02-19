/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_CC_CC_PARSE_TAB_H_INCLUDED
# define YY_CC_CC_PARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef CC_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define CC_DEBUG 1
#  else
#   define CC_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define CC_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined CC_DEBUG */
#if CC_DEBUG
extern int cc_debug;
#endif

/* Token type.  */
#ifndef CC_TOKENTYPE
# define CC_TOKENTYPE
  enum cc_tokentype
  {
    STDSTART = 258,
    CMPSTART = 259,
    STDEND = 260,
    TO = 261,
    SINGLETON = 262
  };
#endif

/* Value type.  */
#if ! defined CC_STYPE && ! defined CC_STYPE_IS_DECLARED

union CC_STYPE
{
#line 25 "cc_parse.y" /* yacc.c:1909  */

	int singleton;

#line 74 "cc_parse.tab.h" /* yacc.c:1909  */
};

typedef union CC_STYPE CC_STYPE;
# define CC_STYPE_IS_TRIVIAL 1
# define CC_STYPE_IS_DECLARED 1
#endif


extern CC_STYPE cc_lval;

int cc_parse (void);

#endif /* !YY_CC_CC_PARSE_TAB_H_INCLUDED  */
