!c
!c     parameters and arrays, necessary for adjoint and hessian calculation
!c
      integer ncmax
      parameter( ncmax = 6200 )

      common /intcontrol/ nc
      integer nc
      common /realcontrol/ xc
      real xc(ncmax)
