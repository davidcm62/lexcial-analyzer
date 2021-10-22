# Notas Práctica 2.1

## Flex
+ automata contenido en yylex()
+ ahora podese chamar a esa funcion en vez de siguienteCompLex()
+ salida igual á practica anterior
+ yylex solo devolve ints
    + 1 opcion: siguienteCompLexico borrar todo e invocar a yylex, meter ese numero no campo da estructura e coller yytext no lexema (asi podo devolver a estructura)
    + 2 opcion: podese modificar a definicion de yylex para que devolva o punteiro a struct
+ podemonos cargar o sistema de entrada (xa o trae)
+ leer minimo ata a 22 ou asi
+ as ER ejecutanse por orden (cuidado co orden)
    + ir do mas especifico ao mas general (punto e floats por ejemplo)
+ contar bien los saltos de linea (por ejemplo saltos de linea dentro de componentes lexicos)
+ no tabs ni linea logica vs fisica (no es necesario)

## Entrega
+ Deixar no readme a version do flex e gcc
+ makefile
+ documentar
+ entregar fichero .l e o compilado



????
er1
er2
* -> error (se chega aqui non reconoce nada: error)????