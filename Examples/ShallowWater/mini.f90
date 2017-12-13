      module mini
!c     minimization parameters, mainly used by the minimization routines
!c     m1qn3
!un      common /mini_m1qn3/ epsg, df1, dxmin, niter, nsim, impres, mode
        SAVE
      integer niter, nsim, impres, mode
      real epsg, df1, dxmin
!c     lbfgs
!un      common /mini_lbfgsl/ eps_grad, pgtol, factr, iprint
      integer iprint
      double precision eps_grad, pgtol, factr
!c
      end module
      
