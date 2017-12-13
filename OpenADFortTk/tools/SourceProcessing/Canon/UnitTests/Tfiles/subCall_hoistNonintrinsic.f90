      real function bar(x)
        real :: x
        bar = x*2
        return
      end function bar
      
      program subCall_hoistIntrinsic
        real :: v
        
        v = 1.4
        call foo(v,bar(v))
        print *,v
        
      contains
        subroutine foo(a,b)
          real :: a,b
          a = b
        end subroutine foo
      end program subCall_hoistIntrinsic

