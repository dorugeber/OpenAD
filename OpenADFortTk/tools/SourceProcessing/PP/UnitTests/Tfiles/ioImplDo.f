      SUBROUTINE head(X, Y)
      TYPE (oadactive) X(1 : 3),p1,p2,y(1:3)
C$OPENAD XXX Template ad_template.f
      read(*,*) __value__(p1),(__value__(x(k)), __value__(y(k)),k=1,3), 
     + __value__(p2)
      write(*,*) __value__(p1),(__value__(x(k))+__value__(y(k)),k=1,3), 
     + __value__(p2)
      END SUBROUTINE
