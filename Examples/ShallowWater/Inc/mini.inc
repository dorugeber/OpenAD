c     minimization parameters, mainly used by the minimization routines
c     m1qn3
      common /mini_m1qn3/ epsg, df1, dxmin, niter, nsim, impres, mode
      integer niter, nsim, impres, mode
      real epsg, df1, dxmin
c     lbfgs
      common /mini_lbfgsl/ eps_grad, pgtol, factr, iprint
      integer iprint
      double precision eps_grad, pgtol, factr
c
      
