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

#ifndef YY_YY_PARSE_H_INCLUDED
# define YY_YY_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_INT = 258,
    TOKEN_CHAR = 259,
    TOKEN_NUMBER = 260,
    TOKEN_STRING = 261,
    TOKEN_IDENTIFIER = 262,
    TOKEN_FUNC = 263,
    KEYWORD_IF = 264,
    KEYWORD_END = 265,
    KEYWORD_ELSE = 266,
    KEYWORD_WHILE = 267,
    KEYWORD_DO = 268,
    KEYWORD_SWITCH = 269,
    KEYWORD_CASE = 270,
    KEYWORD_BREAK = 271,
    KEYWORD_DEFAULT = 272,
    KEYWORD_WHEN = 273,
    KEYWORD_FUNCTION = 274,
    KEYWORD_FROM = 275,
    KEYWORD_BOOL = 276,
    KEYWORD_RETURN = 277,
    KEYWORD_TRUE = 278,
    KEYWORD_FALSE = 279,
    OP_GT = 280,
    OP_LT = 281,
    OP_GE = 282,
    OP_LE = 283,
    OP_EQ = 284,
    OP_NEQ = 285,
    OP_AND = 286,
    OP_OR = 287,
    OP_NEG = 288,
    UMINUS = 289,
    UNEG = 290
  };
#endif
/* Tokens.  */
#define TOKEN_INT 258
#define TOKEN_CHAR 259
#define TOKEN_NUMBER 260
#define TOKEN_STRING 261
#define TOKEN_IDENTIFIER 262
#define TOKEN_FUNC 263
#define KEYWORD_IF 264
#define KEYWORD_END 265
#define KEYWORD_ELSE 266
#define KEYWORD_WHILE 267
#define KEYWORD_DO 268
#define KEYWORD_SWITCH 269
#define KEYWORD_CASE 270
#define KEYWORD_BREAK 271
#define KEYWORD_DEFAULT 272
#define KEYWORD_WHEN 273
#define KEYWORD_FUNCTION 274
#define KEYWORD_FROM 275
#define KEYWORD_BOOL 276
#define KEYWORD_RETURN 277
#define KEYWORD_TRUE 278
#define KEYWORD_FALSE 279
#define OP_GT 280
#define OP_LT 281
#define OP_GE 282
#define OP_LE 283
#define OP_EQ 284
#define OP_NEQ 285
#define OP_AND 286
#define OP_OR 287
#define OP_NEG 288
#define UMINUS 289
#define UNEG 290

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (ast **root, void *scanner);

#endif /* !YY_YY_PARSE_H_INCLUDED  */
