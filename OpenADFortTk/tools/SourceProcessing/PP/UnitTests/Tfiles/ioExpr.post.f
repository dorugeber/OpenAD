      SUBROUTINE head(X, Y)
      use OAD_active
      IMPLICIT NONE
      type(active) :: X(1:2)
      type(active) :: Y(1:1)
C$OPENAD XXX Template ad_template.f
      write(*,*) X(1)%v+y(1)%v
      END SUBROUTINE
