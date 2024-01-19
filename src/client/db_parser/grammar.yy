%require "3.7.4"
%language "c++"
%defines "grammar.hpp"
%output "grammar.cpp"

%define api.parser.class {Parser}
%define api.namespace {project}
// %define api.value.type variant
%parse-param {Scanner* scanner}
%parse-param {dbContext* db_ctx}

/* write out a header file containing the token defines */
%defines

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

#include "db_engine/db_engine.h"

using namespace std;

%}


%union {
    long long       integerVal;
    double          doubleVal;
    std::string*    stringVal;
    std::vector<std::string*>* commandList;
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


%type <commandList> commands
%destructor { delete $$; } commandList


%token SET EXISTS GET TYPE KEYS DEL LIMIT LPAREN RPAREN

%type <stringVal> command key_pattern
%type <integerVal> limit_value

%%

commands: /* пусто */
        | commands command SEMICOLON { $$ = $1; if (!$$) $$ = new std::vector<std::string*>(); $$->push_back($2);
        auto v = *$$;
        std::cout << "vsize = " << v.size();
        for (auto elem : v) {
            std::cout << "elem: " << elem << std::endl;
            auto s = *elem;
            std::cout << s << std::endl;
        } 
     std::cout << "db connection " << db_ctx->dbConnection; 
        send_message(*$2);
}
        | commands SEMICOLON          { $$ = $1; if (!$$) $$ = new std::vector<std::string*>(); }
        ;


command: SET STR_VALUE STR_VALUE { $$ = new std::string(*$2 + *$3); }
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
