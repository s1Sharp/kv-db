%require "3.7.4"
%language "c++"
%defines "grammar.hpp"
%output "grammar.cpp"

%define api.parser.class {Parser}
%define api.namespace {project}
// %define api.value.type variant
%parse-param {Scanner* scanner}
%parse-param {dbContext* db_ctx}


/* add debug output code to generated parser. disable this for release
 * versions. */
%debug


%{
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <FlexLexer.h>

using namespace std;

%}


%union {
    long long       integerVal;
    double          doubleVal;
    std::string*    stringVal;
}


%code requires
{
    namespace project {
        class Scanner;
        class dbContext;
    } // namespace project
} // %code requires

%code
{
    #include "scanner.hpp"
    #define yylex(x) scanner->lex(x)
}


%token SEMICOLON

%token			    END	     0	"end of file"
%token			    EOL		    "end of line"
%token <integerVal> INTEGER		"integer"
%token <doubleVal> 	DOUBLE		"double"
%token <stringVal> 	STR_VALUE	"string"

%destructor { delete $$; } STR_VALUE


%token SET EXISTS GET TYPE KEYS DEL LIMIT LPAREN RPAREN

%type <stringVal> command key_pattern
%type <integerVal> limit_value

%%


commands: /* пусто */
        | commands command SEMICOLON {  std::cout << "db connection " << db_ctx->dbConnection;  }
        ;

command: SET STR_VALUE STR_VALUE
        | EXISTS STR_VALUE
        | GET STR_VALUE
        | TYPE STR_VALUE
        | KEYS key_pattern LIMIT limit_value
        | DEL STR_VALUE
        ;

key_pattern: /* пусто */
           | STR_VALUE key_pattern
           ;

limit_value: /* пусто */
          | INTEGER
          ;

%%

void project::Parser::error(const std::string& msg) {
    std::cerr << "Parser::error: " << msg << '\n';
}
