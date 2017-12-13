      subroutine head(y)
        real y
        external foo
        y = foo(y) ! we better not save this comment!
      end subroutine head

