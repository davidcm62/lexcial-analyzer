%{
    /*definiciones*/
    /*
[:alnum:] [:alpha:] [:blank:]
[:cntrl:] [:digit:] [:graph:]
[:lower:] [:print:] [:punct:]
[:space:] [:upper:] [:xdigit:]
uso: [[:alnum:]]    accion


fai match do mas largo ou do que apareza antes se son igual de largos

accion vacia é igual a ignorar  [patron]    <nada>


podese detectar un error negando as ER (non alfanum non nume etc => error)

{COMMENT}\n?     ter cuidado con estas condiciones \n? para contar as lineas 
    */
    #include "lex.yy.h"
    #include "definitions.h"

    /*Declaración de funciones auxiliares*/
    LexicalComponent* _initLexicalComponent(char *lexeme, int lexicalCompNum);



    int count = 0;
%}

%option noyywrap

ENCODING         #.*coding[:=][[:blank:]]*[-_.[:alnum:]]+[[:blank:]]*.*
COMMENT          #.*
EMPTY_LINE       [[:blank:]]*\n

/*Identificadores e keywords*/
IDENTIFIER       {ID_START}{ID_CONTINUE}*
ID_START         [_[:alpha:]]
ID_CONTINUE      [_[:alnum:]]

/*Integers*/
INTEGER          ({DECINTEGER}|{BININTEGER}|{OCTINTEGER}|{HEXINTEGER})
DECINTEGER       ({NONZERODIGIT}(_?{DIGIT})*)|(0+(_?0)*)
BININTEGER       (0(b|B)(_?{BINDIGIT})+)
OCTINTEGER       (0(o|O)(_?{OCTDIGIT})+)
HEXINTEGER       (0(x|X)(_?{HEXDIGIT})+)
NONZERODIGIT     [1-9]
DIGIT            [[:digit:]]
BINDIGIT         (0|1)
OCTDIGIT         [0-7]
HEXDIGIT         [[:xdigit:]]

/*Floats*/
FLOAT            ({POINTFLOAT})|({EXPONENTFLOAT})
POINTFLOAT       ({DIGITPART})?{FRACTION}|{DIGITPART}\.
FRACTION         \.{DIGITPART}
DIGITPART        {DIGIT}(_?{DIGIT})*
EXPONENTFLOAT    ({DIGITPART}|{POINTFLOAT}){EXPONENT}
EXPONENT         (e|E)[+-]?{DIGITPART}

/*Números imaginarios*/
IMAGNUMBER       ({FLOAT}|{DIGITPART})(j|J)

/*Operadores e delimitadores de 1 caracter*/
OP_DELIM_1_CHAR  \+|-|\*|\/|%|@|&|\||\^|~|\<|\>|\(|\)|\[|\]|\{|\}|,|:|\.|;|@|=

/*Strings*/
STRING           {STRINGPREFIX}?({SHORTSTRING}|{LONGSTRING})
STRINGPREFIX     (r|R|u|U|f|F|fr|Fr|fR|FR|rf|rF|Rf|RF)
SHORTSTRING      (\'{SHORTSTRINGITEM1}*\'|\"{SHORTSTRINGITEM2}*\")
LONGSTRING       (\'\'\'{LONGSTRINGITEM}*\'\'\'|\"\"\"{LONGSTRINGITEM}*\"\"\")
SHORTSTRINGITEM1 ({SHORTSTRINGCHAR1}|{STRINGESCAPESEQ})
SHORTSTRINGITEM2 ({SHORTSTRINGCHAR2}|{STRINGESCAPESEQ})
LONGSTRINGITEM   ({LONGSTRINGCHAR}|{STRINGESCAPESEQ})
SHORTSTRINGCHAR1 [^\\\n\']
SHORTSTRINGCHAR2 [^\\\n\"]
LONGSTRINGCHAR   [^\\]
STRINGESCAPESEQ  \\.



%%
<<EOF>>                     {return _initLexicalComponent("$", EOF_COMP);}
^{EMPTY_LINE}               /*ignorar líneas vacías*/
\n                          {return _initLexicalComponent(yytext, NEWLINE);}
[\\[:blank:]]               /*ignorar espacios e unión de línea explícita*/
{ENCODING}\n?               /*{printf("ENCODING       %s", yytext);}*/
{COMMENT}\n?                /*ignorar comentarios*/

{IDENTIFIER}                {
    printf("INSERT TS?     %s\n", yytext);
    int lexicalCompNum = IDENTIFIER;
    // int *tsValue = searchTS(yytext);
    // if(tsValue == NULL){    //non existe na TS, introduzoo
    //     insertTS(yytext, lexicalCompNum);
    // }else{  //xa existe, devolvo o compoñente léxico gardado
    //     lexicalCompNum = *tsValue;
    //     free(tsValue);
    // }
    return _initLexicalComponent(yytext, lexicalCompNum);
}

{INTEGER}                   {return _initLexicalComponent(yytext, INTEGER);}
{FLOAT}                     {return _initLexicalComponent(yytext, FLOAT);}
{IMAGNUMBER}                {return _initLexicalComponent(yytext, IMAGNUMBER);}

{STRING}                    {return _initLexicalComponent(yytext, STRING);}

{OP_DELIM_1_CHAR}           {return _initLexicalComponent(yytext, yytext[0]);}
\*\*                        {return _initLexicalComponent(yytext, POW);}
\/\/                        {return _initLexicalComponent(yytext, FLOOR_DIV);}
\<\<                        {return _initLexicalComponent(yytext, LEFT_SHIFT);}
\>\>                        {return _initLexicalComponent(yytext, RIGTH_SHIFT);}
:=                          {return _initLexicalComponent(yytext, ASIGN_EVALUATE);}
\<=                         {return _initLexicalComponent(yytext, LESS_EQUALS);}
\>=                         {return _initLexicalComponent(yytext, GREATER_EQUALS);}
==                          {return _initLexicalComponent(yytext, EQUALS);}
!=                          {return _initLexicalComponent(yytext, NOT_EQUALS);}
-\>                         {return _initLexicalComponent(yytext, FUNCTION_NOTATION);}
\+=                         {return _initLexicalComponent(yytext, ADD_EQUALS);}
-=                          {return _initLexicalComponent(yytext, SUB_EQUALS);}
\*=                         {return _initLexicalComponent(yytext, MULT_EQUALS);}
\/=                         {return _initLexicalComponent(yytext, DIV_EQUALS);}
%=                          {return _initLexicalComponent(yytext, MOD_EQUALS);}
@=                          {return _initLexicalComponent(yytext, MATRIX_EQUALS);}
&=                          {return _initLexicalComponent(yytext, AND_EQUALS);}
\|=                         {return _initLexicalComponent(yytext, OR_EQUALS);}
\^=                         {return _initLexicalComponent(yytext, XOR_EQUALS);}
\/\/=                       {return _initLexicalComponent(yytext, FLOOR_DIV_EQUALS);}
\>\>=                       {return _initLexicalComponent(yytext, RS_EQUALS);}
\<\<=                       {return _initLexicalComponent(yytext, LS_EQUALS);}
\*\*=                       {return _initLexicalComponent(yytext, POW_EQUALS);}


<*>.                       {printf("DEFAUUUULT     %s\n", yytext);}
%%


/**
 * Reserva a memoria para a estructura e introduce o lexema 
 * e o compoñente léxico nos campos da mesma.
 */
LexicalComponent* _initLexicalComponent(char *lexeme, int lexicalCompNum){
    LexicalComponent *lexicalComponent = (LexicalComponent*)malloc(sizeof(LexicalComponent));
    lexicalComponent->lexeme = lexeme;
    lexicalComponent->lexicalComp = lexicalCompNum;

    return lexicalComponent;
}

/**
 * Funciones públicas (librería lex.yy.h)
 */

bool initFlex(const char *filename){
    yyin = fopen(filename, "r");

    if(yyin == NULL){
        printf("error IO\n");
        return false;
    }

    return true;
}

void freeFlex(){
    fclose(yyin);
}

void freeLexicalComponent(LexicalComponent *lexicalComponent){
    if(lexicalComponent != NULL){
        free(lexicalComponent);
    }
}