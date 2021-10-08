# Notas Práctica 1.3

## Xeral
+ documentacion da practica
    + comentar o codigo
    + facer un readme (indicar como compilar, se hay restricciones, version de compilador etc)
    + incluir un make ou sh e explicar como se compila (eu meto os 2)
+ non deixar rastros que nos identifiquen no codigo (nombre, autor, etc)
+ non fai falta meter o nombre na carpeta
+ zip
+ na evaluacion (esto é pouco eficiente, basado no noso razonamento, nada de "eu creo...", "eu penso...")

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