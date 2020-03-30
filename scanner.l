%option noyywrap case-insensitive
%{
    #include <stdio.h>
    #include "request.h"
    #include "bison.tab.h"

    int yylex();
    void yyerror(char *s);
%}

%%
[ \t]           { return T_SPACE;   }
"\r\n"          { return T_CRLF;    }

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

int Request_parse(char *buf, int len) 
{
    YY_BUFFER_STATE bp = yy_scan_bytes(buf, len);
    yy_switch_to_buffer(bp);
    yyparse();
    yy_delete_buffer(bp);
    return 0;
}

void yyerror(char* s)
{
    fprintf(stderr, "yylex_error: %s\n", s);
}