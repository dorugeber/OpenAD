c$openad XXX Template ad_template.f
      subroutine head(nx,ny,x,fvec,r)
      integer nx,ny
      double precision r
      double precision x(nx*ny),fvec(nx*ny)
c     **********
c
c     Subroutine head  (dfdcfj)
c
c     This subroutine computes the function of the
c     Flow in a Driven Cavity problem.  The problem is formulated as a 
c     boundary value problem, and the boundary value problem is 
c     discretized by standard finite difference approximations to obtain
c     a system of nonlinear equations.
c
c     The subroutine statement is:
c
c     subroutine dfdcfj(nx,ny,x,fvec,fjac,ldfjac,task,r)
c
c     where
c
c       nx is an integer variable.
c         On entry nx is the number of grid points in the first
c            coordinate direction.
c         On exit nx is unchanged.
c
c       ny is an integer variable.
c         On entry ny is the number of grid points in the second
c            coordinate direction.
c         On exit ny is unchanged.
c
c       x is a double precision array of dimension nx*ny.
c         On entry x specifies the vector x 
c         On exit x is unchanged 
c
c       fvec is a double precision array of dimension nx*ny.
c         On entry fvec need not be specified.
c         On exit fvec contains the function evaluated at x 
c
c       r is a double precision variable.
c         On entry r is the Reynolds number.
c         On exit r is unchanged.
c
c     MINPACK-2 Project. October 1991.
c     Argonne National Laboratory and University of Minnesota.
c     Brett M. Averick.
c
c     Split routine into three routines head, foo, and bar to demonstrate
c     subroutine level checkpointing in the adjoint code. 
c     ACTS Project, September 2004, RWTH Aachen
c
c     **********
      double precision zero,one,two,three,four
      parameter(zero=0.0d0,one=1.0d0,two=2.0d0,three=3.0d0,four=4.0d0)

      integer k,n
      double precision hy,hx,hy2,hx2,nxp1,nyp1
c$openad INDEPENDENT(x)

      n = nx*ny
      nxp1=nx+1
      nyp1=ny+1
      hx = one/nxp1
      hy = one/nyp1
      hy2 = hy*hy
      hx2 = hx*hx

      call foo(nx,ny,x,fvec,r,n,hx,hy,hy2,hx2)

c     Scale the Result.  This is not desired if preconditioning.

      do 70 k = 1, n
         fvec(k) = hx2*hy2*fvec(k)
 70   continue

      return
c$openad DEPENDENT(fvec)
      end

c$openad XXX Template ad_template.f
      subroutine foo(nx,ny,x,fvec,r,n,hx,hy,hy2,hx2)
      integer nx,ny
      double precision r
      double precision x(nx*ny),fvec(nx*ny)
      double precision pbb,pbl,pb,pbr,pll,pl,p,pr,prr,ptl,pt,ptr,ptt,
     +       dpdy,dpdx,plap,pblap,pllap,prlap,ptlap,hy,hx,hy2,hx2
      double precision zero,one,two,three,four
      parameter(zero=0.0d0,one=1.0d0,two=2.0d0,three=3.0d0,four=4.0d0)
      do 60 i = 1, ny
         do 50 j = 1, nx 
            k = (i - 1)*nx + j
            if (i .eq. 1 .or. j .eq. 1) then
               pbl = zero
            else
               pbl = x(k-nx-1)
            endif
            if (i .eq. 1) then
               pb = zero
               pbb = x(k)
            else if (i .eq. 2) then
               pb = x(k-nx)
               pbb = zero
            else
               pb = x(k-nx)
               pbb = x(k-2*nx)
            endif
            if (i .eq. 1 .or. j .eq. nx) then
               pbr = zero
            else
               pbr = x(k-nx+1)
            endif
            if (j .eq. 1) then
               pl = zero
               pll = x(k)
            else if (j .eq. 2) then
               pl = x(k-1)
               pll = zero
            else
               pl = x(k-1)
               pll = x(k-2)
            endif
            p = x(k)
            if (j .eq. nx - 1) then
               pr = x(k+1)
               prr = zero
            else if (j .eq. nx) then
               pr = zero
               prr = x(k)
            else
               pr = x(k+1)
               prr = x(k+2)
            endif
            if (i .eq. ny .or. j .eq. 1) then
               ptl = zero
            else
               ptl = x(k+nx-1)
            endif
            if (i .eq. ny - 1) then
               pt = x(k+nx)
               ptt = zero
            else if (i .eq. ny) then
               pt = zero
               ptt = x(k) + two*hy
            else
               pt = x(k+nx)
               ptt = x(k+2*nx)
            endif
            if (i .eq. ny .or. j .eq. nx) then
               ptr = zero
            else
               ptr = x(k+nx+1)
            endif

            dpdy = (pt - pb)/(two*hy) 
            dpdx = (pr - pl)/(two*hx)

c           Laplacians at each point in the 5 point stencil.

            call bar(pbb,pbl,pb,pbr,pll,pl,p,pr,prr,ptl,pt,ptr,ptt,
     +hy2,hx2,plap,pblap,pllap,prlap,ptlap)

            fvec(k) = (prlap - two*plap + pllap)/hx2 
     +                                  + (ptlap - two*plap + pblap)/hy2 
     +                                - r*(dpdy*(prlap - pllap)/(two*hx)
     +                                  - dpdx*(ptlap - pblap)/(two*hy)) 
   50    continue
   60 continue      
      return
      end


c$openad XXX Template ad_template.f
      subroutine bar(pbb,pbl,pb,pbr,pll,pl,p,pr,prr,ptl,pt,ptr,ptt,
     +hy2,hx2,plap,pblap,pllap,prlap,ptlap)
      double precision pbb,pbl,pb,pbr,pll,pl,p,pr,prr,ptl,pt,ptr,ptt,
     +                 plap,pblap,pllap,prlap,ptlap,hy2,hx2
      double precision zero,one,two,three,four
      parameter(zero=0.0d0,one=1.0d0,two=2.0d0,three=3.0d0,four=4.0d0)
      pblap = (pbr - two*pb + pbl)/hx2 + (p   - two*pb + pbb)/hy2
      pllap = (p   - two*pl + pll)/hx2 + (ptl - two*pl + pbl)/hy2
      plap =  (pr  - two*p  + pl )/hx2 + (pt  - two*p  + pb )/hy2 
      prlap = (prr - two*pr + p  )/hx2 + (ptr - two*pr + pbr)/hy2
      ptlap = (ptr - two*pt + ptl)/hx2 + (ptt - two*pt + p  )/hy2
      return 
      end 
