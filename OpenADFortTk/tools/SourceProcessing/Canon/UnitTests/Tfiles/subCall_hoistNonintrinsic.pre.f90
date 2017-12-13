      real function bar(x)
        use OAD_intrinsics
        real :: x
        bar = x*2
        return
      end function bar
      subroutine oad_s_bar(x,bar)
        use OAD_intrinsics
        real :: x
        real,intent(out) :: bar
        bar = x*2
        return
      end subroutine oad_s_bar
      
      program subCall_hoistIntrinsic
        use OAD_intrinsics
        real :: v
        real :: oad_ctmp0
        
        v = 1.4
        call oad_s_bar(v,oad_ctmp0)
        call foo(v,oad_ctmp0)
        print *,v
        
      contains
        subroutine foo(a,b)
          real :: a,b
          a = b
        end subroutine foo
      end program subCall_hoistIntrinsic

