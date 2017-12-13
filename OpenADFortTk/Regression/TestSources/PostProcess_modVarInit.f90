module m 
  double precision :: m_x=1.0D0
end module

program p 
 use m 
 double precision :: x,y
!$openad INDEPENDENT(x)
 m_x=m_x*x
 y=m_x
!$openad DEPENDENT(y)
end program

