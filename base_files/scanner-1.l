%option noyywrap case-insensitive
%{
    #include <stdio.h>
    #include "bison-1.tab.h"

    int yyparse();
    void yyerror(char *s);
%}

%%
[ \t] { 
    yylval.str = strdup(yytext);
    return T_SPACE;   
}
"\r\n" { 
    yylval.str = strdup(yytext);
    return T_CRLF;    
}

"get" { 
    yylval.str = strdup(yytext);
    return T_METHOD;  
}

"/"[a-zA-Z_-]*  { 
    yylval.str = strdup(yytext);
    return T_PATH;    
}

"http/1."[0|1]  { 
    yylval.str = strdup(yytext);
    return T_VERSION; 
}

. { return *yytext; }
%%

int main()
{
    yyparse();
    return 0;
}

void yyerror(char* s)
{
    fprintf(stderr, "yylex_error: %s\n", s);
}