
      MODULE fft1r_module
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) MIXUP(:)
      ALLOCATABLE MIXUP
      PRIVATE MIXUP
      INTEGER(w2f__i4) SAVED_INDX
      PRIVATE SAVED_INDX
      COMPLEX(w2f__4) SCT(:)
      ALLOCATABLE SCT
      PRIVATE SCT
C
C     **** Initializers ****
C
      DATA SAVED_INDX / -1 /
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE FFT1R_INIT(INDX)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) INDX
        INTENT(IN)  INDX
C
C       **** Local Variables and Functions ****
C
        INTEGER(w2f__i4) IERR
        INTEGER(w2f__i4) ISIGN
        SAVE ISIGN
        INTEGER(w2f__i4) NX
        INTEGER(w2f__i4) NXH
C
C       **** Initializers ****
C
        DATA ISIGN / 0 /
C
C       **** Statements ****
C
        IF(INDX .LT. 0) THEN
          WRITE(*, *) 'indx must be non-negative'
          STOP
          RETURN
        ENDIF
        NX = 2 ** INDX
        NXH = NX / 2
        SAVED_INDX = INDX
        IF(ALLOCATED(MIXUP)) THEN
          DEALLOCATE(MIXUP)
        ENDIF
        IF(ALLOCATED(SCT)) THEN
          DEALLOCATE(SCT)
        ENDIF
        ALLOCATE(MIXUP(NXH), SCT(NXH), STAT= IERR)
        IF(IERR .ne. 0) THEN
          WRITE(*, *) 'allocation error'
          STOP
          RETURN
        ENDIF
        END SUBROUTINE

        SUBROUTINE FFT1R_END
        use w2f__types
        IMPLICIT NONE
C
C       **** Statements ****
C
        SAVED_INDX = -1
        DEALLOCATE(MIXUP, SCT)
        END SUBROUTINE

        SUBROUTINE FFT1R(F, ISIGN)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        REAL(w2f__4) F(1 :)
        INTEGER(w2f__i4) ISIGN
        INTENT(IN)  ISIGN
C
C       **** Local Variables and Functions ****
C
        INTEGER(w2f__i4) NX
        INTEGER(w2f__i4) NXH
C
C       **** Statements ****
C
        NX = SIZE(F)
        NXH = NX / 2
        IF(ISIGN .eq. 0) THEN
          RETURN
        ENDIF
        IF(SAVED_INDX .LT. 0) THEN
          WRITE(*, *) 'fft tables not initialized!'
          STOP
          RETURN
        ENDIF
        END SUBROUTINE
      END

      PROGRAM main
      use w2f__types
      use fft1r_module
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) F(:)
      ALLOCATABLE F
      INTEGER(w2f__i4) INDX
      INTEGER(w2f__i4) NX
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      INDX = 4
      NX = 2 ** INDX
      ALLOCATE(F(NX))
      F( : ) = (/(I, I = 1, NX, 1) /)
      CALL FFT1R_INIT(INDX)
      CALL FFT1R(F, (-1))
      CALL FFT1R_END()
      WRITE(*, *) 'OK'
      
      END PROGRAM
