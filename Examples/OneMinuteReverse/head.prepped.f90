subroutine head(x,y) 
  double precision :: x
  double precision :: y
!$openad INDEPENDENT(x)
  y=tan(x)
!$openad DEPENDENT(y)
end subroutine
