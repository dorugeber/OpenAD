      SUBROUTINE head(X, Y)
      IMPLICIT NONE
      TYPE (oadactive) X(1 : 2)
      TYPE (oadactive) Y(1 : 1)
C$OPENAD XXX Template ad_template.f
      write(*,*) __value__(X(1))+ __value__(y(1))
      END SUBROUTINE
