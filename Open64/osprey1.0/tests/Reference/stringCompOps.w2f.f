C ***********************************************************
C Fortran file translated from WHIRL Fri Dec 11 15:12:56 2009
C ***********************************************************
C ***********************************************************

      PROGRAM p
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      LOGICAL(w2f__i4) LGEV(1 : 3)
      LOGICAL(w2f__i4) LGTV(1 : 3)
      LOGICAL(w2f__i4) LLEV(1 : 3)
      LOGICAL(w2f__i4) LLTV(1 : 3)
C
C     **** Statements ****
C
      LGEV(1) = .FALSE.
      LGEV(2) = .TRUE.
      LGEV(3) = .TRUE.
      LGTV(1) = .FALSE.
      LGTV(2) = .FALSE.
      LGTV(3) = .TRUE.
      LLEV(1) = .TRUE.
      LLEV(2) = .TRUE.
      LLEV(3) = .FALSE.
      LLTV(1) = .TRUE.
      LLTV(2) = .FALSE.
      LLTV(3) = .FALSE.
      IF(.NOT.(LLTV(3) .OR.(LLEV(3) .OR.(LGEV(1) .OR. LGTV(2))))) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'lge', LGEV
        WRITE(*, *) 'lgt', LGTV
        WRITE(*, *) 'lle', LLEV
        WRITE(*, *) 'llt', LLTV
      ENDIF
      
      END PROGRAM
