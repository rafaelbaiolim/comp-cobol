      ******************************************************************
      * Author: GRUPO-COBOL
      * Date: 03/10/2016
      * Purpose: MULTIPLICAR MATRIZ
      * Tectonics: cobc
      ******************************************************************
       IDENTIFICATION DIVISION.
       PROGRAM-ID. MULTIPMATRIZ.
       DATA DIVISION.
       FILE SECTION.
       WORKING-STORAGE SECTION.
       01 MAL PIC 9(1).
       01 MAC PIC 9(1).
       01 MBL PIC 9(1).
       01 MBC PIC 9(1).
       01 ARRAY.
           02 X OCCURS 10 TIMES.
               03 Y OCCURS 10 TIMES.
                   04 A PIC S9(3).
                   04 B PIC S9(3).
                   04 R PIC S9(3).
       01 L_A PIC 9(1).
       01 C_A PIC 9(1).
       01 L_B PIC 9(1).
       01 C_B PIC 9(1).
       01 VALOR PIC S9(3).
       01 ELEMENTO PIC S9(3).
       PROCEDURE DIVISION.
       MAIN-PROCEDURE.
           DISPLAY "Informe a quantidade de linhas da matriz A: ".
           ACCEPT MAL.
           DISPLAY "Informe a quantidade de colunas da matriz A: ".
           ACCEPT MAC.
           DISPLAY "Informe a quantidade de linhas da matriz B: ".
           ACCEPT MBL.
           DISPLAY "Informe a quantidade de colunas da matriz B: ".
           ACCEPT MBC.

           DISPLAY " ".
           DISPLAY " ".

           IF MAC NOT EQUAL MBL THEN
               DISPLAY "A matriz resultante nao existe!"
               DISPLAY ".. Definicao: so existe matriz produto A * B .."
               DISPLAY ".. se o numero de colunas da matriz A .."
               DISPLAY ".. for igual ao numero de linhas da matriz B .."
               STOP RUN
           END-IF.

           DISPLAY "Informe os elementos da matriz A: ".
           PERFORM READ-A VARYING L_A FROM 1 BY 1 UNTIL L_A > MAL
               AFTER C_A FROM 1 BY 1 UNTIL C_A > MAC.

           DISPLAY "Informe os elementos da matriz B: ".
           PERFORM READ-B VARYING L_B FROM 1 BY 1 UNTIL L_B > MBL
               AFTER C_B FROM 1 BY 1 UNTIL C_B > MBC.

           DISPLAY " ".
           DISPLAY " ".

           DISPLAY "A matriz resultante R: ".
           PERFORM CALC VARYING L_A FROM 1 BY 1 UNTIL L_A > MAL
               AFTER C_B FROM 1 BY 1 UNTIL C_B > MBC.

           PERFORM DSP-E VARYING L_A FROM 1 BY 1 UNTIL L_A > MAL
               AFTER C_B FROM 1 BY 1 UNTIL C_B > MBC.

           STOP RUN.

       READ-A.
           DISPLAY "("L_A","C_A") : ", ACCEPT A(L_A, C_A).

       READ-B.
           DISPLAY "("L_B","C_B") : ", ACCEPT B(L_B, C_B).

       CALC.
           MOVE 0 TO ELEMENTO.
           PERFORM CALC-E VARYING C_A FROM 1 BY 1 UNTIL C_A > MAC
               AFTER L_B FROM 1 BY 1 UNTIL L_B > MBL.
           MOVE ELEMENTO TO R(L_A,C_B).

       CALC-E.
           IF (C_A = L_B) THEN
             COMPUTE VALOR = A(L_A,C_A) * B(L_B,C_B)
             ADD VALOR TO ELEMENTO
             END-IF.

       DSP-E.
           DISPLAY "("L_A","C_B") = "R(L_A,C_B).

       END PROGRAM MULTIPMATRIZ.
