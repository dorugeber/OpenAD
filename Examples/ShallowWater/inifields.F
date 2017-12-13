c$openad XXX Template OADrts/ad_template.split.f
      subroutine inifields

      use size
      use parms
      use vars
      use pfields
      use force
      use data
      use weights

      implicit none

!un #include "size.inc"
!un #include "parms.inc"
!un #include "vars.inc"
!un #include "pfields.inc"
!un #include "force.inc"
!un #include "data.inc"
!un #include "weights.inc"
      
      integer ix, iy, ix2, iy2
      
c     set everything to zero
      nt = 0
      ntspinup = 0
      dt = 0.
      start_time = 0.
      dt_dump = 0.
c
c     iteration counter has to be initialized explicitly before starting
c     any minimization, set to some impossible value here.
c      
      iteration = -9999
c
      rini = 0.
      om = 0.
      f0 = 0.
      beta = 0. 
      xstart = 0.
      ystart = 0. 
c     
      xperiodic = .false.
      yperiodic = .false.
      spherical = .false.
      cartesian = .false. 
      quadfric = .false.
c
      suppressio = .false.
      fullio = .false.
c
      initial_grad = .false.
      grad_check = .false.
      optimize = .false.
      calc_hess = .false.
c
      foutname = ' '
      runname = ' '
      depthfile = ' '
      forcingfile = ' '
      uinifile = ' '
      vinifile = ' '
      etainifile = ' '
      ncdatafile = ' '
c
      wf_depth = 0.
      wf_eta = 0.
      wf_u = 0.
      wf_v = 0.
      wf_zonal_transport = 0.
      wf_lapldepth = 0.
      wf_graddepth = 0.
c
c$openad xxx simple loop
      do ix = 0, nx+1
         x(ix) = 0.
      end do
c$openad xxx simple loop
      do iy = 0, ny+1
         y(iy) = 0.
      end do
c$openad xxx simple loop
      do ix = 0, nx
         dx(ix) = 0.
      end do
c$openad xxx simple loop
      do iy = 0, ny
         dy(iy) = 0.
      end do
c$openad xxx simple loop
      do iy = 0, ny+1
         rx(iy) = 0.
      end do
      ry = 0.
c$openad xxx simple loop
      do iy = 0, ny+1
         hy(iy) = 0.
      end do
      dt = 0.
c$openad xxx simple loop
      do iy = 1, ny
         do ix = 1, nx
            uforce(ix,iy) = 0.
            vforce(ix,iy) = 0.
            fcoriu(ix,iy) = 0.
            fcoriv(ix,iy) = 0.
            inidepth(ix,iy) = 0.
            uini(ix,iy) = 0.
            vini(ix,iy) = 0.
            etaini(ix,iy) = 0.
c     
            scaledepth(ix,iy) = 0.
c     data
            u_data(ix,iy) = 0.
            v_data(ix,iy) = 0.
            eta_data(ix,iy) = 0.
            depth_data(ix,iy) = 0.
            weight_u(ix,iy) = 0.
            weight_v(ix,iy) = 0.
            weight_eta(ix,iy) = 0.
            weight_lapldepth(ix,iy) = 0.
            weight_graddepth(ix,iy) = 0.
         end do
      end do
c$openad xxx simple loop
      do iy = 1, ny
c         do iy2 = 1, ny
            do ix = 1, nx
c               do ix2 = 1, ny
                  weight_depth(ix,iy) = 1.
c               end do
            end do
c         end do
      end do
c
      zonal_transport_data = 0.
      weight_zonal_transport = 0.
c
c$openad xxx simple loop
      do ix = 0, nx+1
         do iy = 0, ny+1
            depth(ix,iy) = 0.
            frict(ix,iy) = 0.
            u(ix,iy) = 0.
            v(ix,iy) = 0.
            eta(ix,iy) = 0.
            umask(ix,iy) = 0.
            vmask(ix,iy) = 0.
            etamask(ix,iy) = 0.
            hu(ix,iy) = 0.
            hv(ix,iy) = 0.
            invhu(ix,iy) = 0.
            invhv(ix,iy) = 0.
         end do
      end do
c     
      end ! subroutine inifields
