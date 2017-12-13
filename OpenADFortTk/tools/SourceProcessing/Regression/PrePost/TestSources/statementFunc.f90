program statementFunc
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

  integer i,j,k
  double precision d(10)
  integer a,b
  integer sf
  sf(a,b)=a+b&
       &-2
  
  d(1)=d(sf(i,j)+k)
  d(2)=d((sf(i,j))+k)
  d(3)=d(sf(sf(i,j),j)+k)
  
end subroutine foo

