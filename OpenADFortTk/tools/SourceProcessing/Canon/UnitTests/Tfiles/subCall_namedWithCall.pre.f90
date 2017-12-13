      program subCall_simpleNamed
        use OAD_intrinsics
        real :: v
        real :: oad_ctmp0
        
        v = 1.4
        oad_ctmp0 = sin(v)
        call foo(v,b=oad_ctmp0)
        print *,v
        
      contains
        subroutine foo(a,b)
          real :: a,b
          a = b
        end subroutine foo
      end program subCall_simpleNamed

