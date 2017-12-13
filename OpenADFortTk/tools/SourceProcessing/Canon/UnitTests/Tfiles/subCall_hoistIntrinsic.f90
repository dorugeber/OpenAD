      program subCall_hoistIntrinsic
        real :: v
        
        v = 1.4
        call foo(v,sin(v))
        print *,v
        
      contains
        subroutine foo(a,b)
          real :: a,b
          a = b
        end subroutine foo
      end program subCall_hoistIntrinsic

