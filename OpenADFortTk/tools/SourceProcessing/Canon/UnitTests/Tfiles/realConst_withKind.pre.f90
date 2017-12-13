      subroutine foo(v)
        use OAD_intrinsics
        integer :: w2f__4, w2f__8
        parameter (w2f__4 = kind(0.0))
        parameter (w2f__8 = kind(0.0d0))
        real(w2f__4) v
        real(w2f__4) ifoo
        double precision :: oad_ctmp0

        call oad_s_max(v,6e0_w2f__8,oad_ctmp0)
        ifoo = oad_ctmp0
      end
