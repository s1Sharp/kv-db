%require "3.2"
%language "c++"

%{
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>


using namespace std;

enum TokenType {
    SET,
    EXISTS,
    GET,
    TYPE,
    KEYS,
    DEL,
    KEY,
    LIMIT,
    NUMBER,
    SEMICOLON
};

// extern "C" int yylex(void* yylval_param);
// extern "C" int yylex();
// extern "C" int yyparse();
// extern "C" char* yytext;

int yyerror(const char* msg) {
    std::cerr << "Error: " << msg << std::endl;
    return 1;
}

%}


%union {
    char* str_val;
    int num_val;
}

%token <str_val> KEY
%token <num_val> NUMBER
%token SEMICOLON

%token SET EXISTS GET TYPE KEYS DEL LIMIT

%type <str_val> command key_pattern
%type <num_val> limit_value

%%

commands: /* пусто */
        | commands command SEMICOLON '\n'
        ;

command: SET KEY KEY
        | EXISTS KEY
        | GET KEY
        | TYPE KEY
        | KEYS key_pattern LIMIT limit_value
        | DEL KEY
        ;

key_pattern: /* пусто */
           | KEY key_pattern
           ;

limit_value: /* пусто */
          | NUMBER
          ;

%%

extern int run_parser() {
    yy::parser p;
    p.parse();
    return 0;
}