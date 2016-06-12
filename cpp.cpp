#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { glob_init(); return yyparse(); }

Sym::Sym(string V) { val=V; }
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::head() { return "<"+val+">"; }
string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::dump(int depth) { string S = "\n"+pad(depth)+head();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::eval() {
	Sym*G = glob[val]; if (G) return G;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it) = (*it)->eval();
	return this; }

Sym* Sym::add(Sym*o) { return new Error(head()+"+"+o->head()); }
Sym* Sym::div(Sym*o) { return new Error(head()+"/"+o->head()); }
string Sym::str() { return val; }

Error::Error(string V):Sym(V) { yyerror(V); }

Str::Str(string V):Sym(V) {}
string Str::head() { string S = "'";
	for (int i=0;i<val.length();i++) switch (val[i]) {
		case '\t': S += "\\t"; break;
		case '\n': S += "\\n"; break;
		default: S += val[i];
	}
	return S+"'"; }
Sym* Str::add(Sym*o) { return new Str(val+o->str()); }

Vector::Vector():Sym("[]"){}
string Vector::head() { return val; }
string Vector::str() { string S;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->str();
	return S; }
Sym* Vector::div(Sym*o) {
	Sym* V = new Vector();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++) {
		V->push(*it); V->push(o); }
	V->nest.pop_back();
	return V; }

Op::Op(string V):Sym(V) {}
string Op::head() { return "("+val+")"; }
Sym* Op::eval() {
	if (val=="~") return nest[0]; else Sym::eval();
	if (val=="+") return nest[0]->add(nest[1]);
	if (val=="/") return nest[0]->div(nest[1]);
	return this; }

map<string,Sym*> glob;
void glob_init() {
	glob["MODULE"]	= new Str(MODULE);		// meta info
	glob["ABOUT"]	= new Str(ABOUT);
	glob["AUTHOR"]	= new Str(AUTHOR);
	glob["LICENSE"]	= new Str(LICENSE);
	glob["GITHUB"]	= new Str(GITHUB);
	glob["LOGO"]	= new Str(LOGO);
	glob["sp"]		= new Str(" ");			// string constants
	glob["nl"]		= new Str("\n");
	glob["tab"]		= new Str("\t");
}
