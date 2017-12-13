      program longDataStatementLine
      REAL, DIMENSION(42) :: COEF
      DATA COEF/ 1.D0, 1.D0, -1.D0, 3.D0, -3.D0, 5.D0, 3.D0, -30.D0, 35.D0, &
         15.D0, -70.D0, 63.D0, -5.D0, 105.D0, -315.D0, 231.D0, -35.D0, 315.D0, &
         -693.D0, 429.D0, 35.D0, -1260.D0, 6930.D0, -12012.D0, 6435.D0, 315.D0, &
         -4620.D0, 18018.D0, -25740.D0, 12155.D0, -63.D0, 3465.D0, -30030.D0, &
         90090.D0, -109395.D0, 46189.D0, -693.D0, 15015.D0, -90090.D0, &
         218790.D0, -230945.D0, 88179.D0/

      print *,COEF(5)
      end program longDataStatementLine

