      ******************************************************************
      * Author: GRUPO-COBOL
      * Date: 29/09/2016
      * Purpose: FATORIAL DE N
      * Tectonics: cobc
      ******************************************************************
       IDENTIFICATION DIVISION.
       PROGRAM-ID. FATORIALN.
       DATA DIVISION.
       FILE SECTION.
       WORKING-STORAGE SECTION.
       77 fact pic 9(15) comp.
       77 n pic 99.
       77 i pic 99.
       77 ist pic XX.
       77 factst pic X(18).

       PROCEDURE DIVISION.
       MAIN-PROCEDURE.
           MOVE 8 to n
           MOVE 0 to i
           MOVE 1 to fact
           PERFORM UNTIL i GREATER THAN n
               MOVE i TO ist
               MOVE fact to factst
               DISPLAY ist "! = " factst
               ADD 1 TO i
               MULTIPLY i BY fact
                 ON SIZE ERROR DISPLAY "Valor inserido muito grande!"
               END-MULTIPLY
           END-PERFORM.
           STOP RUN.
       END PROGRAM FATORIALN.
