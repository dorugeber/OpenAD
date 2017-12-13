module m 
      real,private :: x, y
      integer,private :: nt=12, nc=20
      public
      interface foo
        module procedure foo_i
      end interface
contains 
      subroutine foo_i()
      x=1.0
      y=1.0
      if (nt*nc==240 .and. x*y==1.0) then
        print *,'OK'
      else
        print *,nt*nc, x*y
      end if
      end subroutine
end module 

program privateQ
      use m 
      call foo()
end program
