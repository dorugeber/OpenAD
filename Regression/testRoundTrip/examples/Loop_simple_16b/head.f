c$openad XXX Template ad_template.f
      subroutine lagran(I,X,A,SP,lag)
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
c$openad xxx simple loop
      DO K = 1, SP, 1
        IF(I .ne. K) THEN
          DENOM = DENOM*(A(I)-A(K))
          NUMER = NUMER*(X-A(K))
        ENDIF
      END DO
      lag = NUMER/DENOM
      RETURN
      RETURN
      end subroutine

c$openad XXX Template ad_template.f
	subroutine head(x,y)
	  double precision, dimension(4), intent(in) :: x
	  double precision, dimension(1), intent(out) :: y
c$openad INDEPENDENT(x)
	  call lagran(2,2.0D0,x,4,y(1))
c$openad DEPENDENT(y)
	end subroutine
