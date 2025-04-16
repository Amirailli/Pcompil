
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     MAINPRGM = 258,
     VAR = 259,
     BEGINPG = 260,
     ENDPG = 261,
     LET = 262,
     ATDEF = 263,
     CONST = 264,
     INPUT = 265,
     OUTPUT = 266,
     TYPEINT = 267,
     TYPEFLOAT = 268,
     ENTIER = 269,
     ENTIERSIGNE = 270,
     FLOAT = 271,
     IDF = 272,
     PLUS = 273,
     MINUS = 274,
     TIMES = 275,
     DIV = 276,
     AND = 277,
     OR = 278,
     NOT = 279,
     EQ = 280,
     NEQ = 281,
     IE = 282,
     SE = 283,
     I = 284,
     S = 285,
     OUVEREBLOC = 286,
     FERMETBLOC = 287,
     CROCHETOUVERT = 288,
     CROCHETFERME = 289,
     PARENTHESEOUVERT = 290,
     PARENTHESEFERME = 291,
     PVG = 292,
     DEUXPOINT = 293,
     VRG = 294,
     EGAL = 295,
     AFFECTATION = 296,
     IF = 297,
     THEN = 298,
     ELSE = 299,
     DO = 300,
     WHILE = 301,
     FOR = 302,
     FROM = 303,
     TO = 304,
     STEP = 305,
     CHAINE = 306,
     VAR_IDF = 307
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 17 "Projet.y"

    int entier;
    char* str;
    float flottant ;



/* Line 1676 of yacc.c  */
#line 112 "Projet.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


