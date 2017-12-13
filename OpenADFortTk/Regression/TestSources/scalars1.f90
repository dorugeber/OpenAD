!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A simple program with scalar addition and multiplication
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine head(x, y) 
  double precision, intent(in) :: x
  double precision, intent(inout) :: y
  double precision t1, t2

  t1=x*y
  t2=t1*x+y
  y=t1+t2

end subroutine


program scalars1

   double precision a,b
   a=1.3
   b=2.5
   call head(a,b)
   print *,b
end program
