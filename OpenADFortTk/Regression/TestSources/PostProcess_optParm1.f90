subroutine foo(a,b,c,d)
  double precision :: a,c
  double precision, optional :: b,d
  c=a
  if (present(b).and.present(d)) then 
    d=b
  end if
end subroutine

program optParm1
  interface 
     subroutine foo(a,b,c,d)
       double precision :: a,c
       double precision, optional :: b,d
     end subroutine
  end interface 
  double precision :: x,y,u,v
!$openad INDEPENDENT(x)
!$openad DEPENDENT(y)
  x=2.0D0
  call foo(a=x,c=u)
  call foo(u,x,y,v)
  print *, y,v
end program
