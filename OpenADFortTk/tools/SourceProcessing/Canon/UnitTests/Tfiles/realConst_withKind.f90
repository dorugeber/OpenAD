      subroutine foo(v)
        integer :: w2f__4, w2f__8
        parameter (w2f__4 = kind(0.0))
        parameter (w2f__8 = kind(0.0d0))
        real(w2f__4) v
        real(w2f__4) ifoo

        ifoo = MAX(v,6e0_w2f__8)
      end
