%{
    #include <stdio.h>
    #include "request.h"

    int yylex();
    void yyerror(char *s);

    extern RequestLine* ReqLine;
%}

%debug

// 定义yylval是一个union，它可以存储字符串，也可以存储整数，但并不是同时存储.
%union {
    // int num;
    char* str;
    struct RequestLine* request_line;
}

%token <str> T_METHOD T_VERSION T_PATH T_SPACE T_CRLF
%type <request_line> request_line
%type <str> method path version

%%
request_line:
    method T_SPACE path T_SPACE version T_CRLF {
        $$ = (RequestLine*)malloc(sizeof(RequestLine));
        $$->method = $1;
        $$->path = $3;
        $$->version = $5;
        ReqLine = $$;
        YYACCEPT;
    };

method:
    T_METHOD { 
        printf("parse-method: %s \n", $1); 
        $$ = $1; 
    };

path:
    T_PATH { 
        printf("parse-path: %s \n", $1); 
        $$ = $1; 
    };

version:
    T_VERSION { 
        printf("parse-version: %s \n", $1); 
        $$ = $1; 
    };

%%