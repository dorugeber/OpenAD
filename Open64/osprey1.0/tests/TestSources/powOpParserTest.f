      SUBROUTINE m_gasoilkinetics(  X_v)
      IMPLICIT NONE
      double precision OpenAD_Symbol_0
      double precision  X_v(1 : *) 
      INTEGER(4) L_Y1
      L_Y1 = 1
      OpenAD_Symbol_0 = (2.0D00*(X_v(L_Y1)**(2.0D00-INT(1))))
C illustrates a parsing error of the above statement
      end subroutine

      program p 
      double precision :: X(2)
      call m_gasoilkinetics(X)
      print *, 'OK'
      end program
