program singleVariable
  double precision  :: x
!$openad INDEPENDENT(x)
  x=2.0
  x=x*2.0
!$openad DEPENDENT(x)
end program
