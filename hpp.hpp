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
	virtual Sym* eval();
	virtual Sym* eq(Sym*);
	virtual Sym* at(Sym*);
	virtual Sym* add(Sym*);
	virtual Sym* div(Sym*);
	virtual string str();
};
extern map<string,Sym*> glob;
extern void glob_init();

struct Var: Sym { Var(string,Sym*); string head(); };

struct Error: Sym { Error(string); };

struct Str: Sym { Str(string); string head(); Sym*add(Sym*); };

struct Vector: Sym { Vector(); string head(); Sym*div(Sym*); string str(); };

struct Op: Sym { Op(string); string head(); Sym*eval(); };

typedef Sym*(*FN)(Sym*);
struct Fn: Sym { Fn(string,FN); FN fn ; Sym*at(Sym*); };

struct Dir: Sym { Dir(string); static Sym* dir(Sym*); string head(); };
struct File: Sym { File(string); static Sym* file(Sym*); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_EXPERT
