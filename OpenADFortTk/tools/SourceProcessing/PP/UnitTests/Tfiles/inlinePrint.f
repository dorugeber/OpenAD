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
        COUNT = 1
        MASK(1 : N, 1 : M) = 0
        DO I = 2, (N +(-1)), 1
          DO J = 2, (M +(-1)), 1
C           $OpenAD$ INLINE push_s0(subst)
            CALL push_s0(NMSB(I, J))
          END DO
        END DO
        END SUBROUTINE
