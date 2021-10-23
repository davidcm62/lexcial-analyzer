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
    int count = 0;

    #define YY_DECL int yylex(float a, float b)
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
<<EOF>>                     {printf("EOF %f %f\n",a,b); return count;}
^{EMPTY_LINE}               /*ignorar líneas vacías*/
\n                          {printf("NEW_LINE\n");}
[\\[:blank:]]               /*ignorar espacios e unión de línea explícita*/
{ENCODING}\n?               {printf("ENCODING       %s", yytext);}
{COMMENT}\n?                /*ignorar comentarios*/

{IDENTIFIER}                {printf("IDENTIFIER     %s\n", yytext);}

{INTEGER}                   {printf("INTEGER        %s\n", yytext);}
{FLOAT}                     {printf("FLOAT          %s\n", yytext);}
{IMAGNUMBER}                {printf("IMAGNUMBER     %s\n", yytext);}

{STRING}                    {printf("STRING         %s\n", yytext);}

{OP_DELIM_1_CHAR}           {printf("OP_1_CHAR      %d\n", yytext[0]);}
\*\*                        {printf("OP2            %s\n", yytext);}
\/\/                        {printf("OP2            %s\n", yytext);}
\<\<                        {printf("OP2            %s\n", yytext);}
\>\>                        {printf("OP2            %s\n", yytext);}
:=                          {printf("OP2            %s\n", yytext);}
\<=                         {printf("OP2            %s\n", yytext);}
\>=                         {printf("OP2            %s\n", yytext);}
==                          {printf("OP2            %s\n", yytext);}
!=                          {printf("OP2            %s\n", yytext);}
-\>                         {printf("OP2            %s\n", yytext);}
\+=                         {printf("OP2            %s\n", yytext);}
-=                          {printf("OP2            %s\n", yytext);}
\*=                         {printf("OP2            %s\n", yytext);}
\/=                         {printf("OP2            %s\n", yytext);}
%=                          {printf("OP2            %s\n", yytext);}
@=                          {printf("OP2            %s\n", yytext);}
&=                          {printf("OP2            %s\n", yytext);}
\|=                         {printf("OP2            %s\n", yytext);}
\^=                         {printf("OP2            %s\n", yytext);}
\/\/=                       {printf("OP3            %s\n", yytext);}
\>\>=                       {printf("OP3            %s\n", yytext);}
\<\<=                       {printf("OP3            %s\n", yytext);}
\*\*=                       {printf("OP3            %s\n", yytext);}


<*>.                       {printf("DEFAUUUULT     %s\n", yytext);}
%%















void init(){
    FILE *fp;
    fp = fopen("input.py","r");
    yyin = fp;
}

void closeeeee(){
    fclose(yyin);
}