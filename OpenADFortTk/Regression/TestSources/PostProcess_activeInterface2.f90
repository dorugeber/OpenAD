module modA
  interface
     subroutine foo (a, b, c, i)
       double precision :: a(i), b(i), c(i+1)
       integer, intent(in) :: i
     end subroutine
  end interface
end module

subroutine foo (a, b, c, i)
  implicit none
  double precision :: a(i), b(i), c(i+1)
  integer :: i
  a(1)=a(1)*2.0
  return
end subroutine

program activeInterface2
  use modA
  double precision, dimension(2) :: x,y,b
  double precision, dimension(3) :: c 
!$openad INDEPENDENT(x)
!$openad DEPENDENT(y)
  x(1)=2.0
  x(2)=3.0
  call foo(x,b,c,2)
  y=x
  if (y(1).eq.4.0) then 
    print *, 'OK' 
  end if
end program
