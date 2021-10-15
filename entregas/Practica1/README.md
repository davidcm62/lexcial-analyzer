# Instrucciones Práctica 1 Compiladores
Este documento recolle a información básica sobre o proxecto necesaria para poder entender como se estrutura e para poder realizar a compilación e execución do mesmo.

Para comprender como funciona o código, apórtanse os comentarios necesarios dentro do mesmo para unha mellor comprensión.

## Estructura de carpetas
O proxecto encóntrase dentro da carpeta **src/**. Dentro están todos os ficheiros de código fonde e cabeceiras necesarias. Atoparemos as seguintes carpetas e ficheiros:
 
 + *main.c*: invoca a execución do compilador
 + **ts/**: contén a táboa de símbolos e a estructura de datos (neste caso un hashmap con encadeamento)
 + **syntactic/**: contén o analizador sintáctico
 + **lexical/**: contén o analizador léxico
 + **inputSystem/**: contén o sistema de entrada
 + **erro/**: contén a xestión de erros do compilador
 + **common/**: contén ficheiros de definicións comúns ao resto de módulos do compilador

## Compilación
Xunto co código fonde, adxúntase un *Makefile* que permite compilar o proxecto.

Tendo a seguinte estructura de carpetas:
+ **src/**
    + *main.c:
    + **ts/**
    + **syntactic/**
    + **lexical/**
    + **inputSystem/**
    + **erro/**
    + **common/**
+ *Makefile*
+ *wilcoxon.py*

Para compilar, executar o seguinte comando situándose no mesmo nivel que o *Makefile*:
```
make
```

Isto creará un directorio chamado **bin/** que terá no seu interior o código obxecto e o executable do compilador.


## Execución
Para executar o compilador, estando na carpeta raíz:
```
./bin/compiler wilcoxon.py
```
**O segundo argumento do compilador é obrigatorio e correspondese co path ao ficheiro de código fonte**. O path pode ser relativo ou absoluto. 