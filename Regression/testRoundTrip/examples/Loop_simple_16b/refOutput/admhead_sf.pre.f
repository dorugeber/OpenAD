!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine lagran(I,X,A,SP,lag)
      use OAD_intrinsics
      IMPLICIT NONE
      integer :: I
      double precision :: X
      double precision :: A(1:4)
      integer :: SP
      double precision,intent(out) :: lag
      double precision DENOM
      INTEGER K
      double precision NUMER
      NUMER = 1.0D00
      DENOM = 1.0D00
C$openad xxx simple loop
      DO K = 1, SP, 1
        IF (I.ne.K) THEN
          DENOM = DENOM*(A(I)-A(K))
          NUMER = NUMER*(X-A(K))
        ENDIF
      END DO
      lag = NUMER/DENOM
      RETURN
      RETURN
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(4), intent(in) :: x
        double precision, dimension(1), intent(out) :: y
        integer :: oad_ctmp0
        double precision :: oad_ctmp1
        integer :: oad_ctmp2
C$openad INDEPENDENT(x)
        oad_ctmp0 = 2
        oad_ctmp1 = 2.0D0
        oad_ctmp2 = 4
        call lagran(oad_ctmp0,oad_ctmp1,x,oad_ctmp2,y(1))
C$openad DEPENDENT(y)
      end subroutine
