%{
#include "grammar.hpp"
#include "scanner.hpp"

#define YY_DECL int project::Scanner::lex(project::Parser::semantic_type *yylval)
// workaround for bug in flex 2.6.4
#define yypanic(X) (void)(X)

using namespace project;

%}
 
%option c++ interactive noyywrap noyylineno nodefault outfile="scanner.cpp"


set     [Ss][Ee][Tt]
exists  [Ee][Xx][Ii][Ss][Tt][Ss]
get     [Gg][Ee][Tt]
type    [Tt][Yy][Pp][Ee]
keys    [Kk][Ee][Yy][Ss]
del     [Dd][Ee][Ll]

%%
"("       return Parser::token::LPAREN;
")"       return Parser::token::RPAREN;

{set}     { std::cout << "SET" << std::endl; return Parser::token::SET; }
{exists}  { std::cout << "EXISTS" << std::endl; return Parser::token::EXISTS; }
{get}     { std::cout << "GET" << std::endl; return Parser::token::GET; }
{type}    { std::cout << "TYPE" << std::endl; return Parser::token::TYPE; }
{keys}    { std::cout << "KEYS" << std::endl; return Parser::token::KEYS; }
{del}     { std::cout << "DEL" << std::endl; return Parser::token::DEL; }
[A-Za-z0-9_,.-]+  { std::cout << "STR_VALUE:" << yytext << std::endl; yylval->stringVal = new std::string(yytext, yyleng); return Parser::token::STR_VALUE; }
"limit" { std::cout << "LIMIT" << std::endl; return Parser::token::LIMIT; }
[0-9]+  { std::cout << "INTEGER" << std::endl; yylval->integerVal = atol(yytext); return Parser::token::INTEGER; }
";"     { std::cout << "SEMICOLON" << std::endl; return Parser::token::SEMICOLON; }
\n|\r   { std::cout << "EOL" << std::endl; return Parser::token::EOL; }
[ \t]   { /* Пропуск пробелов и табуляции */ }
.       { std::cerr << "Unknown token: " << yytext << std::endl; }


%%

int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}

int run_parser() {
    project::Scanner scanner{ std::cin, std::cerr };
    project::Parser parser{ &scanner };
    std::cout.precision(10);
    parser.parse();
    return 0;
}