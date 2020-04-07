%option noyywrap case-insensitive
%{
    #include <stdio.h>

    int yylex();
    void yyerror(char *s);
%}

%%
[ \t]  { printf("space: %s \n", yytext); }
"\r\n" { printf("rn: %s \n", yytext); }

"get" { 
    printf("method: %s \n", yytext);
}

"/"[a-zA-Z_-]*  { 
    printf("uri: %s \n", yytext);  
}

"http/1."[0|1]  { 
    printf("version: %s \n", yytext);
}

. { printf("other: %s \n", yytext); }
%%

int main()
{
    yylex();
    return 0;
}

void yyerror(char* s)
{
    fprintf(stderr, "yylex_error: %s\n", s);
}