      module m 
        double precision :: g 
      end module 
      module m1 
        double precision :: g1
      end module 
      subroutine bar()
      use m 
      use m1
      call foo()
      call foo1()
      end subroutine
      subroutine foo()
      use m 
      use m1
      g1=g*2.0
      end subroutine
      subroutine foo1()
      use m 
      use m1 
      g=g1*3.0
      end subroutine
      subroutine baz()
      use m
      use m1
      use all_globals_mod
          g=x(1)
          call bar()
          y(1)=g
       end subroutine
      subroutine head() 
      use all_globals_mod
      use m 
      use m1
c$openad INDEPENDENT(x)
          call baz()
c$openad DEPENDENT(y)
       end subroutine
