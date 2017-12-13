module OAD_intrinsics
end module
program statementFunc
  use OAD_intrinsics
  implicit None

  integer :: a,b,c
  double precision :: d(10)
  a = 1
  b = 2
  c = 3
  call foo(a,b,c,d)

  write(*,*) 'd = ',d

end program statementFunc

subroutine foo(i,j,k,d)
  use OAD_intrinsics

  integer i,j,k
  double precision d(10)
  integer a,b
  integer sf
  sf(a,b)=a+b-2
  
  d(1) = d((i+j-2)+k)
  d(2) = d(((i+j-2))+k)
  d(3) = d(((i+j-2)+j-2)+k)
  
end subroutine foo

