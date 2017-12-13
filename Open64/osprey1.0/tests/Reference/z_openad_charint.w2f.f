
      PROGRAM main
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) POS
      CHARACTER(10) STR1
      SAVE STR1
      CHARACTER(10) STR2
C
C     **** Initializers ****
C
      DATA STR1 / 'undefined ' /
C
C     **** Statements ****
C
      POS = SCAN('fortran', 'tr') + SCAN('fortran', 'tr', BACK = .TRUE.
     > )
      IF(POS .eq. 8) THEN
        POS = VERIFY('aabb', 'ab') + VERIFY('abc', 'ab')
        IF(POS .eq. 3) THEN
          IF(LLE('undefined', STR1)) THEN
            STR2 = 'abc'
            IF(LGT(STR1, STR2)) THEN
              POS = LEN(STR2) - LEN_TRIM(STR2)
              IF(POS .eq. 7) THEN
                WRITE(*, *) 'OK'
              ELSE
                WRITE(*, *) 'failed'
              ENDIF
            ELSE
              WRITE(*, *) 'failed'
            ENDIF
          ELSE
            WRITE(*, *) 'failed'
          ENDIF
        ELSE
          WRITE(*, *) 'failed'
        ENDIF
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
