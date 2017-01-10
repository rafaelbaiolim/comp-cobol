      ******************************************************************
      * Author: GRUPO-COBOL
      * Date: 10/01/2017
      * Purpose: BUBBLE SORT
      * Tectonics: cobc
      ******************************************************************
       IDENTIFICATION DIVISION.
       PROGRAM-ID. BUBBLESORT.
       DATA DIVISION.
       FILE SECTION.
       WORKING-STORAGE SECTION.
       01 NUM PIC 9(3).
       01 CNT PIC 9(3).
       01 IDX PIC 9(3).
       01 TMP PIC 9(3).
       01 ARRAY.
           02 X OCCURS 100 TIMES.
               03 VET PIC S9(3).
       01 ELEM PIC 9(3).
       01 FLAG pic X.
           88 FLAGTRUE VALUE 'Y'.
           88 FLAGFALSE VALUE 'N'.
       PROCEDURE DIVISION.
       MAIN-PROCEDURE.
           DISPLAY "Informe a quantidade de elementos do vetor: ".
           ACCEPT NUM.

           DISPLAY " ".

           DISPLAY "Informe os elementos do vetor".

           PERFORM READ-VET VARYING ELEM FROM 1 BY 1 UNTIL ELEM > NUM.

           DISPLAY " ".
           DISPLAY " ".

           PERFORM BSORT.

           DISPLAY "Resultado : " WITH NO ADVANCING.
           PERFORM DSP-E VARYING ELEM FROM 1 BY 1 UNTIL ELEM > NUM.

           STOP RUN.

       READ-VET.
           DISPLAY "Elemento "ELEM" : ", ACCEPT VET(ELEM).

       BSORT.
           MOVE NUM TO CNT.

           PERFORM WITH TEST AFTER UNTIL FLAGFALSE
               SET FLAGFALSE TO TRUE
               SUBTRACT 1 FROM CNT
               PERFORM VARYING IDX FROM 1 BY 1
                   UNTIL IDX > CNT
                   IF VET(IDX) > VET(IDX + 1)
                       MOVE VET(IDX) TO TMP
                       MOVE VET(IDX + 1) TO VET(IDX)
                       MOVE TMP TO VET(IDX + 1)
                       SET FLAGTRUE TO TRUE
                 END-IF
              END-PERFORM
           END-PERFORM.

       DSP-E.
           DISPLAY " "VET(ELEM)"; " WITH NO ADVANCING.

       END PROGRAM BUBBLESORT.
