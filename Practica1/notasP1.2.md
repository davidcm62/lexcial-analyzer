# Notas Práctica 1.2

## Requisitos á hora de escoller unha TS
+ buscar, insertar, crear, liberar, imprimir
+ se xa insertas non existe e sustituyes se existe, fai falta buscar?

## Analizador léxico
+ Autómatas para reconocer (un para numeros, operadores, strings, etc)

q0 -> q1 -> ... -> qf para cada autómata

+ O analizador léxico é un autómata que engloba o resto
+ Programar o autómata como unha tabla (inferno)
+ Podemos usar unha estructura condicional
    + Switch, if else...
    + Variable estado (empeza en 0)
    + Miro o seguinte caracter, se é alfanumerico paso ao 1 (a reconoces cadenas alfanumericas)
    + Se encontrase un digito, é un número
    + Vou cambiando de estado segundo o que me chegue
    + Problema:
        + Moitos estados
        + Se encontro un error teño que buscar o estado
        + Se quero cambiar algo teño que buscar toda a estructura e pode ser lioso
+ Podemos simplificar o cacho do switch (que detecta as cadenas alfanumericas por ejemplo), nunha funcion
    + Facer unha funcion por cada automata intermedio
    + Podo saber onde esta o erro (que funcion falla)
+ 0b0x3
    + Unha opcion: error lexico (non é un 0b0010101... ben formado) (lexical error)
    + Outra leer 0b0 e x3, numero e identificador, o sintactico devolve cousas ben formadas, sin error lexico, pero hai error gramatical, que se o come o analizador sintactico (syntax error)
+ Os compiladores suelen deixar os errores ao sintactico (syntax error)
+ Xa hai funciones que reconocen en C numeros, hex, etc.
+ Algunhos comp recoñecense no caracter final, outros no seguinte
    + No final: ( ) { } , comentarios de 3 lineas, strings, <=, etc
    + Despois: comentarios de unha linea, identificadores, palabras reservadas, numeros, operadores de 1 caracter
+ Python solo devolve comp lexico ao final dunha linea logica (non fai falta)
