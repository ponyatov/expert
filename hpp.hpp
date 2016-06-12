#ifndef _H_EXPERT
#define _H_EXPERT

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;
#include "meta.hpp"

struct Sym {
	string val;
	Sym(string);
	vector<Sym*> nest; void push(Sym*);
	virtual string head(); string pad(int); string dump(int=0);
};

struct Str: Sym { Str(string); string head(); };

struct Vector: Sym { Vector(); string head(); };

struct Op: Sym { Op(string); string head(); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_EXPERT
