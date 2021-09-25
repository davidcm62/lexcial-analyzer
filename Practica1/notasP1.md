# Notas Práctica 1

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
    + (non é obligatorio, pero se o fas realista ten recompensa, podes sacar un 10 igual sen facelo realista)
+ os comentarios descartanse


## Ficheros
+ fichero de definiciones: definiciones.h (define os componentes lexicos, podo leelo para saber os componentes lexicos do lenguaje)
+ analizador lexico: lexico.c e lexico.h (funcion seguinte_comp_lexico)
+ tabla de simbolos: gardar (keyword e identificadores) e buscar
    + TS.h (solo ten gardar e buscar, non a definicion do hash etc), TS.c
+ main.c

## Entrega
+ carpeta cos archivos, o wilcoxon.py, un README.md e un make