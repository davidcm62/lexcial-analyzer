# Notas Práctica 1.1

+ https://docs.python.org/3/reference/lexical_analysis.html
+ leo todo o archivo
+ objetivo: sacar por pantalla os componentes lexicos (imprimilos)
    + <300,"import"> etc
    + imprimir a TS ao principio de todo e ao final
+ entrega compilable en ubuntu
+ un componente lexico é un numero enteiro
    + o analizador lexico entrega ao sintactico unha secuencia de numeros
    + o numero elijoo eu
    + definir #define as palabras reservadas
    + meterlle #define _IMPORT 300 para evitar conflictos (_)
+ o analizador sintactico pidelle ao analizador lexico o seguinte compoñente lexico
    + e este devolvelle o componente lexico
    + devolve un punteiro a unha estructura (int para o comp. lexico e un *char co lexema)
+ no analizador sintactico un bucle que pide e imprime hasta que lle manden un error
    + productor-consumidor (é o consumidor)
+ scipy.stats -> scipy, . , stats (3 cousas distintas)
+ scipy é un identificador
    + #define IDENTIFICADOR 301
+ na TS hai ("import",_IMPORT) (keyword, numero asociado)
    + podense ler dun ficheiro ou cargalas mediante un array de palabras e metelas cun for
    + mellor a opcion dun array
+ analizador lexico invoca a TS cunha funcion de busqueda
    + a TS devolve o mismo componente lexico que lle devolve ao sintactico
    + a TS devolve o numero asociado ao lexema
+ no main antes de empezar a analizar chamo a inicializar() TS (mete as keywords)
    + o main, ao principio, é o encargado de crear as estructuras de datos
    + ao final do main liberanse as estructuras de datos
    + no media chama ao analizador sintactico para que empece a analizar
+ na practica 1 non hai estructura de bloque (todas as variables que se chaman igual son a mesma)
+ o tamaño do hash e eso ten que ser justificable
+ buscase se existe scipy na TS, e como non existe, insertase ("scipy", _IDENTIFICADOR)
+ o punto xa esta consumido, e hai que devolvelo
+ leemos o punto
    + meter o . ; { } ( ) na TS non sirve para nada
    + en vez de usar define pa todos, para estes usamos o seu valor ascii 
        (a non ser que sean varios caracteres, que se meterian no ficheiro de definicion)
    + non se inserta, pero si que se devolve
    + para o punto, devolve <46, ".">
+ leemos stats, añadimolo a TS
+ cada vez que termina una linea fisica, entregamos un fin de linea como un componente lexico
    + (non é obligatorio, pero se o fas realista ten recompensa (ter en conta linea loxica e fisica), podes sacar un 10 igual sen facelo realista)
+ os comentarios descartanse (non generan componentes lexicos)


## Ficheros
+ fichero de definiciones: definiciones.h (define os componentes lexicos, podo leelo para saber os componentes lexicos do lenguaje)
+ analizador lexico: lexico.c e lexico.h (funcion seguinte_comp_lexico)
+ tabla de simbolos: gardar (keyword e identificadores) e buscar
    + TS.h (solo ten gardar e buscar, non a definicion do hash etc), TS.c
+ main.c

## Entrega
+ carpeta cos archivos, o wilcoxon.py, un README.md e un make

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