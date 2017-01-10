      ******************************************************************
      * Author: GRUPO-COBOL
      * Date: 10/01/2017
      * Purpose: BUSCA BINARIA
      * Tectonics: cobc
      ******************************************************************
       IDENTIFICATION DIVISION.
       PROGRAM-ID. BUSCABINARIA.
       DATA DIVISION.
       FILE SECTION.
       WORKING-STORAGE SECTION.
       01 NUM PIC 9(3).
       01 CHAVE PIC 9(3).
       01 INF PIC 9(3).
       01 SUP PIC 9(3).
       01 MEIO PIC 9(3).
       01 ARRAY.
           02 X OCCURS 100 TIMES.
               03 VET PIC S9(3).
       01 ELEM PIC 9(3).
       PROCEDURE DIVISION.
       MAIN-PROCEDURE.
           DISPLAY "Informe a quantidade de elementos do vetor: ".
           ACCEPT NUM.

           DISPLAY " ".

           DISPLAY "Informe os elementos do vetor".
           DISPLAY "(estes devem ser inseridos ordenados)".

           PERFORM READ-VET VARYING ELEM FROM 1 BY 1 UNTIL ELEM > NUM.

           DISPLAY "Informe a chave : ".
           ACCEPT CHAVE.

           DISPLAY " ".
           DISPLAY " ".

           MOVE 1 TO INF.
           COMPUTE SUP = NUM.

           PERFORM BUSCA UNTIL INF > SUP.

           DISPLAY "Elemento nao encontrado!".

           STOP RUN.

       READ-VET.
           DISPLAY "Elemento "ELEM" : ", ACCEPT VET(ELEM).

       BUSCA.
           COMPUTE MEIO = (INF + SUP) / 2.
           IF (CHAVE = VET(MEIO)) THEN
               DISPLAY "Elemento encontrado no indice "MEIO". "
               STOP RUN
           ELSE
               IF (CHAVE < VET(MEIO)) THEN
                   COMPUTE SUP = MEIO - 1
               ELSE
                   COMPUTE INF = MEIO + 1
                   END-IF
           END-IF.

       END PROGRAM BUSCABINARIA.
