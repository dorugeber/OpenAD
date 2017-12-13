        SUBROUTINE OpenAD_MASKER(N,M,THCK)
        INTEGER N
        INTEGER M
        REAL THCK(1 : N, 1 : M)
        INTEGER I=2
        INTEGER J=2
C       $OpenAD$ BEGIN REPLACEMENT 2
C           $OpenAD$ INLINE push_s2(subst)
            CALL push_s2(THCK(I +(-1) : I + 1, J +(-1) : J + 1))
C       $OpenAD$ END REPLACEMENT
        END SUBROUTINE
