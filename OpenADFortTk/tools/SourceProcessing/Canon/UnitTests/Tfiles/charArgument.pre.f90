      subroutine foo(x)
        use OAD_intrinsics
        character(*) x
        print *,x
      end subroutine
      program p
        use OAD_intrinsics
        character*3 :: oad_ctmp0
        oad_ctmp0 = "bar"
        call foo(oad_ctmp0)
      end program p
