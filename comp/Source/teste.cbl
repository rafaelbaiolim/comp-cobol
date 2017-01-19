      ******************************************************************
      * Author: GRUPO-COBOL
      * Date: 10/01/2017
      * Purpose: MATRIZ TRANSPOSTA
      * Tectonics: cobc
      ******************************************************************
       IDENTIFICATION DIVISION.
       PROGRAM-ID. MATRIZTRANSP.
       DATA DIVISION.
       FILE SECTION.
       WORKING-STORAGE SECTION.
       01 MAL PIC 9(1).
       01 MAC PIC 9(1).
       01 ARRAY.
           02 X OCCURS 10 TIMES.
               03 Y OCCURS 10 TIMES.
                   04 A PIC S9(3).
                   04 R PIC S9(3).
       01 L_A PIC 9(1).
       01 C_A PIC 9(1).
       PROCEDURE DIVISION.
       MAIN-PROCEDURE.
           DISPLAY "Informe a quantidade de linhas da matriz: ".
           ACCEPT MAL.
           DISPLAY "Informe a quantidade de colunas da matriz: ".
           ACCEPT MAC.

           DISPLAY " ".
           DISPLAY " ".

           DISPLAY "Informe os elementos da matriz: ".
           PERFORM READ-A VARYING L_A FROM 1 BY 1 UNTIL L_A > MAL
               AFTER C_A FROM 1 BY 1 UNTIL C_A > MAC.

           DISPLAY " ".
           DISPLAY " ".

           DISPLAY "A matriz transposta: ".
           PERFORM CALC VARYING L_A FROM 1 BY 1 UNTIL L_A > MAL
               AFTER C_A FROM 1 BY 1 UNTIL C_A > MAC.

           PERFORM DSP-E VARYING C_A FROM 1 BY 1 UNTIL C_A > MAC
               AFTER L_A FROM 1 BY 1 UNTIL L_A > MAL.

           STOP RUN.

       READ-A.
           DISPLAY "("L_A","C_A") : ", ACCEPT A(L_A, C_A).

       CALC.
           MOVE A(L_A,C_A) TO R(C_A,L_A).

       DSP-E.
           DISPLAY "("C_A","L_A") = "R(C_A,L_A).

       END PROGRAM MATRIZTRANSP.
