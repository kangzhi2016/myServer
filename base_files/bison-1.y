%{
    #include <stdio.h>

    int yylex();
    void yyerror(char *s);

    typedef struct RequestLine {
        char* uri;
        char* method;
        char* version;
    } RequestLine;

    RequestLine* ReqLine;
%}

// 定义yylval是一个union，它可以存储字符串，也可以存储整数，但并不是同时存储.
%union {
    // int num;
    char* str;
    struct RequestLine* request_line;
}

%token <str> T_METHOD T_VERSION T_PATH T_SPACE T_CRLF
%type <request_line> request_line
%type <str> method uri version

%%
request_line:
    method T_SPACE uri T_SPACE version T_CRLF {
        $$ = (RequestLine*)malloc(sizeof(RequestLine));
        $$->method = $1;
        $$->uri = $3;
        $$->version = $5;
        ReqLine = $$;
        YYACCEPT;
    };

method:
    T_METHOD { 
        printf("method: %s \n", $1); 
        $$ = $1; 
    };

uri:
    T_PATH { 
        printf("path: %s \n", $1); 
        $$ = $1; 
    };

version:
    T_VERSION { 
        printf("version: %s \n", $1); 
        $$ = $1; 
    };

%%

// int main()
// {
//     yyparse();
//     return 0;
// }

// int Request_parse(char *buf, int len) {
//     YY_BUFFER_STATE bp = yy_scan_bytes(buf, len);
//     yy_switch_to_buffer(bp);
//     yyparse();
//     yy_delete_buffer(bp);
//     return 0;
// }

// void yyerror(char* s)
// {
//     fprintf(stderr, "yyparse_error: %s\n", s);
// }