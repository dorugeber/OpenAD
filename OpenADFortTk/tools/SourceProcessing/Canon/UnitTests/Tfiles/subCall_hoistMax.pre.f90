      program subCall_hoistMax
        use OAD_intrinsics
        real*8 :: v8,r
        real :: v4
        double precision :: oad_ctmp0
        real :: oad_ctmp1
        
        v8 = 1.4D0
        v4 = 1.2
        call oad_s_max(v8,2.3D0,oad_ctmp0)
        call foo(oad_ctmp0,r)
        print *,r
        call oad_s_max(v4,2.3,oad_ctmp1)
        call bar(oad_ctmp1,r)
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
