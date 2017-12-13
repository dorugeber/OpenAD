module modA
  interface foo_i
    subroutine foo(sizeA,a)
      integer sizeA, a(1:sizeA+1)
    end subroutine
  end interface
end module

subroutine foo(sizeAdef,adef)
integer :: sizeAdef 
integer :: adef(1:sizeAdef+1)
adef(1)=adef(1)+1
end subroutine

program p 
  use modA
  integer, parameter ::  n=1
  integer :: a(1:n+1)
  a(1)=1
  a(2)=2
  call foo_i(n,a)
  if (a(1)==2 .and. a(2)==2) then  
    print *,'OK'
  else 
    print *,'failure'
  end if
end program
