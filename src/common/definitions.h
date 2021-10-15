#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/**
 * Palabras reservadas
 */
#define FALSE 300
#define AWAIT 301
#define ELSE 302
#define IMPORT 303
#define PASS 304
#define NONE 305
#define BREAK 306
#define EXCEPT 307
#define IN 308
#define RAISE 309
#define TRUE 310
#define CLASS 311
#define FINALLY 312
#define IS 313
#define RETURN 314
#define AND 315
#define CONTINUE 316
#define FOR 317
#define LAMBDA 318
#define TRY 319
#define AS 320
#define DEF 321
#define FROM 322
#define NONLOCAL 323
#define WHILE 324
#define ASSERT 325
#define DEL 326
#define GLOBAL 327
#define NOT 328
#define WITH 329
#define ASYNC 330
#define ELIF 331
#define IF 332
#define OR 333
#define YIELD 334
#define PRINT 335

/**
 * Identificadores e tipos de datos
 */
#define IDENTIFIER 336
#define NEWLINE 337
#define STRING 338
#define INTEGER 339
#define FLOAT 340

/**
 * Operadores
 */
#define INCREMENT 341	        // ++
#define DECREMENT 342		    // --
#define POW 343     	        // **
#define FLOOR_DIV 344		    // //
#define ADD_EQUALS 345		    // +=
#define SUB_EQUALS 346		    // -=
#define MULT_EQUALS 347		    // *=
#define DIV_EQUALS 348		    // /=
#define POW_EQUALS 349		    // **=
#define FLOOR_DIV_EQUALS 350    // //=
#define MOD_EQUALS 351		    // %=
#define MATRIX_EQUALS 352	    // @=
#define ASIGN_EVALUATE 353	    // :=
#define AND_EQUALS 354		    // &=
#define OR_EQUALS 355		    // |=
#define XOR_EQUALS 356		    // ^=
#define EQUALS 357		        // ==
#define GREATER_EQUALS 358	    // >=
#define LESS_EQUALS 359		    // <=
#define NOT_EQUALS 360		    // !=
#define LEFT_SHIFT 361		    // <<
#define RIGTH_SHIFT 362		    // >>
#define LS_EQUALS 363	        // <<=
#define RS_EQUALS 364	        // >>=
#define FUNCTION_NOTATION 365   // ->

/**
 * Fin de ficheiro ($)
 */
#define EOF_COMP 366

/**
 * Compoñente léxico inválido (sirve para comprobar errores)
 */
#define INVALID_LEXICAL_COMPONENT -1

#endif /* DEFINITIONS_H */