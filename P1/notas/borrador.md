# Notas Práctica 1.1

+ objetivo: sacar por pantalla os componentes lexicos (imprimilos)
    + <300,"import"> etc
    + imprimir a TS ao principio de todo e ao final

+ entrega compilable en ubuntu
+ buscase se existe scipy na TS, e como non existe, insertase ("scipy", _IDENTIFICADOR)
+ leemos o punto
    + meter o . ; { } ( ) na TS non sirve para nada
    + en vez de usar define pa todos, para estes usamos o seu valor ascii 
        (a non ser que sean varios caracteres, que se meterian no ficheiro de definicion)
    + non se inserta, pero si que se devolve
    + para o punto, devolve <46, ".">
+ leemos stats, añadimolo a TS
+ cada vez que termina una linea fisica, entregamos un fin de linea como un componente lexico
+ os comentarios descartanse (non generan componentes lexicos)


## Lexico
+ Ao encontrar EOF devolver o compLexico fin fichero (o $ da clase)

## TS
+ faime falta buscar e insertar
    + buscar lexema, se existe devolvo xa o compLexico que esta gardado, se non teño que meter o novo lexema


## Sistema de Entrada
+ funcion siguienteCaracter()
+ os automatas van pendindo caracteres, ata chegar a aceptacion
+ doble buffer con centinela
    + 2 array de char
    + 3 centinela (EOF)
+ cargase como un buffer ciclico (A,B,A,B,A,B,A,B...)
+ abrir o ficheiro e cargar o primeiro bloque (8 espacios, o ultimo EOF) -> solo o primeiro
+ necesito cargar 7 caracteres (fread)
+ puntero delantero e inicio apuntan ao primeiro caracter do primeiro bloque (mellor punteiros ca indices para delantero e incio: inicio = buffer[0])
+ delantero vaise movendo ata encontrar o espacio despois de import
+ o tam maximo do lexema seria tam buffer
+ cada vez que chego ao EOF do bloque cargo o seguinte bloque no seguinte buffer
+ cando o lexema excede o tam do buffer o lexico ten que generar un error (tipo: LEXEMA_EXCEDE_TAM_MAX)
+ a primeira vez facer o tam do buffer igual ca wilcoxon e despois ir decrementando se funciona
+ o buffer deberia ter o tam do identificador mas grande de wilcoxon
+ o error de lexema demasiado largo detectao o sistema de entrada (é quen esta limitado) cando a diferencia entre punteiros supera o tam do buffer
+ cando hay error non se genera codigo ejecutable
+ esto é error porque poderia dar a identificadores erroneos
+ para devolver un caracter, fago retroceder o punteiro delantero e movo inicio ata esa posicion
+ se ao devolver volvo ao EOF do bloque anterior, implicaria cargar outro bloque no siguiente e saltariamonos un bloque
+ para ir lendo e gardando os caracteres do lexema que estou reconocendo o mellor é tanto facer un malloc de 8 en 8 como unha copia no sistema de entrada???? preguntarlle esto a manu

## Errores
+ Centralizar gestion errores
+ Desacoplala: errores.c e errores.h
    + Funcion para gestion de errores (mensaje en funcion de codigo)
    + 1 = Error abrir fichero
    + etc