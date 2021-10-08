#include <stdio.h>
#include "TS.h"

#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>


char* seguinteCaracterCodigoFonte(FILE *file){
    char *c = (char*)malloc(sizeof(char));
    *c = fgetc(file);

    if(*c == EOF){
        return NULL;
    }
    
    return c;
}

void retrocederNcarateres(FILE *file, long offset){
    fseek(file, -offset, SEEK_CUR);
}

void retroceder1caracter(FILE *file){
    retrocederNcarateres(file, 1);
}


// \ explicit line joining
// cadenas alfanumericas (ids keywords)
// integers
    // floating point 
// operadores
    // delimitadores
// strings simples
// sintngs dobles
// comentarios 1 linea e 3 lineas
// tabs?

// import scipy.stats as st
//     tmp = sorted(differences)
//     if N <= 30:

char* alfanum(const char *input){
    int estado = 0;
    int actual = 0;
    int analize = 1;

    while (analize){
        printf("%c[a:%d e:%d] ",input[actual],actual, estado);
        switch (estado){
            case 0:
                if(isalpha(input[actual]) || input[actual] == '_'){
                    actual++;
                    estado = 1;
                }else{
                    estado = 2;
                }
                break;
            case 1:
                if(isalnum(input[actual]) || input[actual] == '_'){
                    actual++;
                }else{
                    estado=3;
                }
                break;
            case 2:
                //error
                analize = 0;
                break;
            case 3:
                //fin
                analize = 0;
                actual--;
                break;
        }
    }
    printf("\nactual %d   estado %d (%s)\n",actual,estado, estado==3? "final": "no final");
    for(int i=0;i<=actual;i++){
        printf("%c",input[i]);
    }
    printf("\n");
    printf("\n");
    
    // printf("%s\n",input);
    return NULL;
}

void comentario1linea(const char *input){
    int estado = 0;
    int actual = 0;
    int analize = 1;

    printf("comentario?\n");
    while (analize){
        // printf("%c[a:%d e:%d] ",input[actual],actual, estado);
        switch (estado){
            case 0:
                if(input[actual] == '#'){
                    actual++;
                    estado = 1;
                }else{
                    estado = 2;
                }
                break;
            case 1:
                if(input[actual] == '\n'){
                    estado = 3;
                }else{
                    actual++;
                }
                break;
            case 2:
                //error
                analize = 0;
                break;
            case 3:
                //fin
                analize = 0;
                actual--;
                break;
        }
    }
    printf("\nactual %d   estado %d (%s)\n",actual,estado, estado==3? "final": "no final");
    for(int i=0;i<=actual;i++){
        printf("%c",input[i]);
    }
    printf("\n");
    printf("\n");
    
    // printf("%s\n",input);
}

char* nextCharFromSourceFile(){
    //opcion: ir devolvendo o char e o offset e co offset poderia volver ao principio
    /*
        poderia ir lendo o archivo, recibir ese offset como parametro
        que cada automata controle canto lee
        en caso de fallo retrocedo SEEK_CUR -posicion leidas
        asi a proxima vez que chame a seguinteComp o cursor estará onde quedou a ultima vez
    */
    //poderia ter unha funcion en cada automata que sexa retrocedeXoffset no archivo cada vez que acabe
    return NULL;
}

void automata(FILE *file){
    bool found = false;
    int estado = 0;
    char *currentChar;
    int compLexico = -1;
    char *lexema = NULL;

    while(!found){
        switch (estado){
            case 0:
                currentChar = seguinteCaracterCodigoFonte(file);

                //se é EOF levar a un estado error que acabe o bucle
                //if c==null error

                if(currentChar == NULL){
                    estado = 11;
                }else{
                    printf("%c\n",*currentChar);
                }
                free(currentChar);
                // if(isalpha(*currentChar) || *currentChar == '_'){
                //     estado = 1;
                // }else if(isdigit(*currentChar)){
                //     estado = 2;
                // }
                //etc
                break;
            case 1:
                lexema = alfanum("asdasda");
                if(lexema != NULL){
                    compLexico = 123123123;
                    estado = 10; //final
                }else{
                    //probaría outro caso (ainda que se chega aqui xa sei que sí vale)
                }
                break;
            case 10:
                //meter comprobar e eso na TS
                found = true;
                break;
            case 11:
                printf("ACABOSE\n");
                found = true;
                break;
        }
    }

    // return do comp lexico ou null se houbo error
}

int main(int argc, char const **argv){
    TS *ts = crearTS();
    // imprimirTS(*ts);
    liberarTS(ts);
    
    const char *input1 = "import scipy.stats as st";
    const char *input2 = "6import scipy.stats as st";
    const char *input3 = "62";
    const char *input4 = "(asdasdasd)";
    const char *input5 = "#(asdasdasd)\n";
    const char *input6 = "# (asdasdasd)\n";
    
    // alfanum(input1);
    // alfanum(input2);

    // comentario1linea(input1);
    // comentario1linea(input5);
    // comentario1linea(input6);


    FILE *file = fopen("./python/wilcoxon.py", "r");
    // automata(file);
    fclose(file);
    
    return 0;
}