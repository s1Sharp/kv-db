%option c++

%{
#include "y.tab.hpp"


extern yy::parser::value_type yylval;

using token = yy::parser::token;
%}

%%
set     { return token::SET; }
exists  { return token::EXISTS; }
get     { return token::GET; }
type    { return token::TYPE; }
keys    { return token::KEYS; }
del     { return token::DEL; }
"<"[a-zA-Z0-9]+">"  { yylval.str_val = strdup(yytext + 1); return token::KEY; free(yylval.str_val); }
"limit" { return token::LIMIT; }
[0-9]+  { yylval.num_val = atoi(yytext); return token::NUMBER; }
";"     { return token::SEMICOLON; }
\n|\r   { /* Обработка символа новой строки */ }
[ \t]   { /* Пропуск пробелов и табуляции */ }
.       { std::cerr << "Unknown token: " << yytext << std::endl; }

%%

int yywrap() {
    return 1; // Завершение анализа при достижении конца файла
}
