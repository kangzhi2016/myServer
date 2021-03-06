%option noyywrap case-insensitive
%{
    #include <stdio.h>
    #include "bison.tab.h"

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

%%

// int main()
// {
//     yyparse();
//     return 0;
// }

int Request_parse(char *buf, int len) 
{
    //开启debug
    // yydebug = 1;
    
    int parse_res;
    printf("buf-len: %d \n", len);
    printf("start parse... \n");

    YY_BUFFER_STATE bp = yy_scan_bytes(buf, len);
    yy_switch_to_buffer(bp);
    parse_res = yyparse();
    yy_delete_buffer(bp);

    return parse_res;
}

void yyerror(char* s)
{
    fprintf(stderr, "yylex_error: %s\n", s);
}

//编译命令
// flex scanner.l &&  bison -d bison.y && gcc -g -W -Wall main.c socket.c request.c lex.yy.c bison.tab.c