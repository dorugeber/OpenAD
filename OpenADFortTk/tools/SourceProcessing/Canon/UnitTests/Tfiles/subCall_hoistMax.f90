      program subCall_hoistMax
        real*8 :: v8,r
        real :: v4
        
        v8 = 1.4D0
        v4 = 1.2
        call foo(max(v8,2.3D0),r)
        print *,r
        call bar(max(v4,2.3),r)
        print *,r
        
      contains
        subroutine foo(a,b)
          double precision :: a,b
          b = a
        end subroutine foo
        subroutine bar(a,b)
          real :: a
          double precision :: b
          b = a
        end subroutine bar
      end program subCall_hoistMax
