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
#pragma once

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
    long long           integerVal;
    double              doubleVal;
    ICommand*           commaVal;
    std::string*        stringVal;
    std::vector<std::string*>* commandList;
}


%code requires
{
    #include "db_parser/db_command.h"

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


%destructor { delete $$; } commaVal

%destructor { delete $$; } STR_VALUE

%type <commandList> commands
%destructor { delete $$; } commandList


%token SET EXISTS GET TYPE KEYS DEL LIMIT LPAREN RPAREN

%type <commaVal>   command
%type <stringVal>  key_pattern
%type <integerVal> limit_value

%%

input: command SEMICOLON { std::cout << "input command ENDLINE " << $1 << std::endl; YYABORT; }
      ;

command: SET STR_VALUE STR_VALUE { 
    $$ = new SetCommand(*$2, *$3); std::cout << $$; 
}
        | EXISTS STR_VALUE END
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
