%{
    #include "./lex.yy.h"
    #include "../common/definitions.h"
    #include "../error/error.h"
    #include "../ts/TS.h"
    #include "./stack.h"

    /*Declaración de funciones auxiliares*/
    LexicalComponent* _initLexicalComponent(char *lexeme, int lexicalCompNum);
    LexicalComponent* _handleError();

    /*Variables globales*/
    char *_filename;
    long _lines = 1;
    Stack *_stack;
%}

/*Opciones flex*/
%option noyywrap
%option nounput
%option noinput

/*Variables incio condición*/
%x START_COND_LONGSTRING1
%x START_COND_LONGSTRING2
%x START_COND_COMMENT


/*Ctes*/
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
OP_DELIM_1_CHAR  \+|-|\*|\/|%|@|&|\||\^|~|\<|\>|,|:|\.|;|@|=

/*Strings*/
STRING           {STRINGPREFIX}?{SHORTSTRING}
STRINGPREFIX     (r|R|u|U|f|F|fr|Fr|fR|FR|rf|rF|Rf|RF)
SHORTSTRING      (\'{SHORTSTRINGITEM1}*\'|\"{SHORTSTRINGITEM2}*\")
SHORTSTRINGITEM1 ({SHORTSTRINGCHAR1}|{STRINGESCAPESEQ})
SHORTSTRINGITEM2 ({SHORTSTRINGCHAR2}|{STRINGESCAPESEQ})
SHORTSTRINGCHAR1 [^\\\n\']
SHORTSTRINGCHAR2 [^\\\n\"]
STRINGESCAPESEQ  \\.

/*String mal formado*/
BADSTRING        {STRINGPREFIX}?{BADSHORTSTRING}
BADSHORTSTRING   ([']{SHORTSTRINGITEM1}*)|(["]{SHORTSTRINGITEM2}*)

%%
<<EOF>>                     {return _initLexicalComponent("$", EOF_COMP);}
^{EMPTY_LINE}               {_lines++; BEGIN(INITIAL);}  //ignorar líneas en blanco
\n                          {
    _lines++;
    if(isEmptyStack(*_stack)){  //pila vacía? devolvo línea física
        return _initLexicalComponent(yytext, NEWLINE);
    }else{  //pila con elementos: línea lóxica
        BEGIN(INITIAL); 
    }
}
[[:blank:]]                 /*ignorar espacios e unión de línea explícita*/
\\\n                        /*ignorar espacios e unión de línea explícita*/

#                           {BEGIN(START_COND_COMMENT);}
<START_COND_COMMENT>{
    .*      {BEGIN(INITIAL);}             //ignora comentarios sin salto de linea
    .*\n    {_lines++; BEGIN(INITIAL);}    //ignora comentarios acabados en \n
}

{IDENTIFIER}                {
    int lexicalCompNum = IDENTIFIER;    //número de compoñente léxico por defecto
    int *tsValue = searchTS(yytext);    //comprobo se existe o lexema na TS
    if(tsValue == NULL){    //non existe na TS, introduzoo
        insertTS(yytext, lexicalCompNum);
    }else{  //xa existe, devolvo o compoñente léxico gardado
        lexicalCompNum = *tsValue;
        free(tsValue);
    }
    return _initLexicalComponent(yytext, lexicalCompNum);
}

{INTEGER}                   {return _initLexicalComponent(yytext, INTEGER);}
{FLOAT}                     {return _initLexicalComponent(yytext, FLOAT);}
{IMAGNUMBER}                {return _initLexicalComponent(yytext, IMAGNUMBER);}

{STRING}                    {return _initLexicalComponent(yytext, STRING);}
{STRINGPREFIX}?[']{3}       {yymore(); BEGIN(START_COND_LONGSTRING1);}
<START_COND_LONGSTRING1>{
    [^\\'\n]    {yymore();}
    \n          {_lines++; yymore();}
    \\.         {yymore();}
    ['][']?     {yymore();}
    [']{3}      { 
        BEGIN(INITIAL);
        return _initLexicalComponent(yytext, STRING);
    }
}
{STRINGPREFIX}?["]{3}       {yymore(); BEGIN(START_COND_LONGSTRING2);}
<START_COND_LONGSTRING2>{
    [^\\"\n]    {yymore();}
    \n          {_lines++; yymore();}
    \\.         {yymore();}
    ["]["]?     {yymore();}
    ["]{3}      { 
        BEGIN(INITIAL);
        return _initLexicalComponent(yytext, STRING);
    }
}

\(|\[|\{                    {
    pushStack(_stack, yytext[0]); //meto o elemento na pila para controlar as líneas lóxicas
    return _initLexicalComponent(yytext, yytext[0]);
}
\)|\]|\}                    {
    if(
        (yytext[0] == ')' && getTopStack(*_stack) == '(') ||
        (yytext[0] == ']' && getTopStack(*_stack) == '[') ||
        (yytext[0] == '}' && getTopStack(*_stack) == '{')
    ){
        popStack(_stack); //elemento que cerra, co cal retiro da pila
    }
    return _initLexicalComponent(yytext, yytext[0]);
}
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

[$?`!]+                     {_handleError();}   //caracteres inválidos*/
(0(b|B))/[^01]              {_handleError();}   //bindigit mal formado*/
(0(x|X))/[^0-9a-fA-F]       {_handleError();}   //hexdigit mal formado*/
(0(o|O))/[^0-7]             {_handleError();}   //octdigit mal formado*/
{BADSTRING}                 {_handleError();}   //short string mal formado*/
<*>.                        {_handleError();}   //erro calquera caracter non identificado
%%

/**
 *  Imprime o erro e devolve un compoñente inválido co lexema inválido leído.
 */
LexicalComponent* _handleError(){
    handleErrorWithFileStats(LEXICAL, yytext, _filename, _lines);
    return _initLexicalComponent(yytext, INVALID_LEXICAL_COMPONENT);
}

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
 * =========================================
 */

bool initFlex(const char *filename){
    yyin = fopen(filename, "r");    //abro archivo

    //copio o nome do ficheiro
    _filename = (char*)malloc((strlen(filename) + 1)*sizeof(char));
    strcpy(_filename, filename);

    if(yyin == NULL){
        //error ficheiro
        handleError(IO);
        return false;
    }

    //incializamos a pila para os saltos de línea lóxicos
    _stack = initStack();

    return true;
}

void freeFlex(){
    //libero recursos
    free(_filename);
    fclose(yyin);
    yylex_destroy();
    freeStack(_stack);
}

void freeLexicalComponent(LexicalComponent *lexicalComponent){
    if(lexicalComponent != NULL){
        free(lexicalComponent);
    }
}