        SUBROUTINE OpenAD_MASKER(N, M, THCK, NMSB, MASK, COUNT)
        INTEGER N
        INTEGER M
        REAL THCK(1 : N, 1 : M)
        REAL NMSB(1 : N, 1 : M)
        INTEGER MASK(1 : N, 1 : M)
        INTEGER COUNT
C
C       **** Local Variables and Functions ****
C
        INTEGER I
        INTEGER J
C
C       **** Statements ****
C
C       $OpenAD$ BEGIN REPLACEMENT 2
        COUNT = 1
        MASK(1 : N, 1 : M) = 0
        DO I = 2, (N +(-1)), 1
          DO J = 2, (M +(-1)), 1
            IF((NMSB(I, J) .GT. 0.0D00) .OR. ANY(THCK(I +(-1) : I + 1,
     >  J +(-1) : J + 1) .ne. 0.0D00)) THEN
              MASK(I, J) = COUNT
              COUNT = (COUNT + 1)
            ENDIF
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(NMSB(I, J))
C           $OpenAD$ INLINE push_s2(subst)
            CALL push_s2(THCK(I +(-1) : I + 1, J +(-1) : J + 1))
          END DO
        END DO
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(N)
C       $OpenAD$ INLINE push_i(subst)
        CALL push_i(M)
        COUNT = (COUNT +(-1))
C       $OpenAD$ END REPLACEMENT
        END SUBROUTINE
