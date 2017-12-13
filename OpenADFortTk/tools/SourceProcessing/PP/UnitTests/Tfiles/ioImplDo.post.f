      SUBROUTINE head(X, Y)
      use OAD_active
      type(active) :: X(1:3),p1,p2,y(1:3)
C$OPENAD XXX Template ad_template.f
      read(*,*) p1%v,(x(k)%v,y(k)%v,k=1,3),p2%v
      write(*,*) p1%v,(x(k)%v+y(k)%v,k=1,3),p2%v
      END SUBROUTINE
