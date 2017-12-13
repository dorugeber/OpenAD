subroutine foo(a,b,c,d)
  double precision :: a,c
  double precision, optional :: b,d
  c=2*a
  if (present(b).and.present(d)) then 
    d=3*b
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
  x=2.0D0
  call foo(a=x,c=u)
  call foo(u,x,y,v)
  if ( y==4*x .and. v==3*x) then 
    print *, 'OK'
  else 
    print *,y,v,'failed'
  end if
end program
