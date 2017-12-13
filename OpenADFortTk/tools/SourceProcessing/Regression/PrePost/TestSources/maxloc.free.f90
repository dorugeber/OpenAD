subroutine foo(i)
  print *,i
end subroutine

program maxlocTest
  implicit none
  double precision x(4),y(2,2)
  x=(/1.0, 2.0, 3.0, 1.5/)
  y=reshape(x,(/2,2/))
  call foo(maxloc(x))
  call foo(maxloc(y))
  contains 
    subroutine foo(i)
      integer :: i(:)
      print *,i
    end subroutine foo
end program 

