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
    */
    int count = 0;
%}
MAYUSCULA   [A-Z]

%%
[0-9]           /*Ignora numeros*/
{MAYUSCULA}     {printf("%s capital letter\n", yytext);count++;}
\n              {printf("SALTO DE LINEA\n");}
\.              {printf("PUNTO\n");}
.               /**/
<<EOF>>         {printf("EOF\n"); return count;}
%%

void init(){
    FILE *fp;
    fp = fopen("README.md","r");
    yyin = fp;
}

void closeeeee(){
    fclose(yyin);
}