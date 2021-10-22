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

    */
    int count = 0;
%}

ENCODING        #.*coding[:=][[:blank:]]*[-_.[:alnum:]]+[[:blank:]]*.*
COMMENT         #.*
EMPTY_LINE      [[:blank:]]*\n

/*Identificadores e keywords*/
IDENTIFIER      {ID_START}{ID_CONTINUE}*
ID_START        [_[:alpha:]]
ID_CONTINUE     [_[:alnum:]]

/*Integers*/
INTEGER         ({DECINTEGER}|{BININTEGER}|{OCTINTEGER}|{HEXINTEGER})
DECINTEGER      ({NONZERODIGIT}[_{DIGIT}]*)|(0+[_0]*)
BININTEGER      (0(b|B)[_{BINDIGIT}]+)
OCTINTEGER      (0(o|O)[_{OCTDIGIT}]+)
HEXINTEGER      (0(x|X)[_{HEXDIGIT}]+)
NONZERODIGIT    [1-9]
DIGIT           [[:digit:]]
BINDIGIT        (0|1)
OCTDIGIT        [0-7]
HEXDIGIT        [[:xdigit:]]



%%
<<EOF>>                     {printf("EOF\n"); return count;}
^{EMPTY_LINE}               /*ignora líneas vacías*/
\n                          {printf("NEW_LINE\n");}
[\\[:blank:]]               /*ignorar espacios e saltos lógicos explícitos*/
{ENCODING}\n                {printf("ENCODING       %s", yytext);}
{COMMENT}\n                 {printf("COMMENT        %s", yytext);}
{IDENTIFIER}                {printf("IDENTIFIER     %s\n", yytext);}
{INTEGER}                   {printf("INTEGER        %s\n", yytext);}
%%















void init(){
    FILE *fp;
    fp = fopen("input.py","r");
    yyin = fp;
}

void closeeeee(){
    fclose(yyin);
}