c     parameter fields, like depth, grid etc.
      common /pfields/ fcoriu, fcoriv, depth, frict, 
     &     hu, hv, invhu, invhv
c     coriolis parameter, depth, and friction coefficient 
c     defined at eta grid point 
      real fcoriu(nx,ny) 
      real fcoriv(nx,ny) 
      real depth(0:nx+1,0:ny+1)
      real hu(0:nx+1,0:ny+1), hv(0:nx+1,0:ny+1)
      real invhu(0:nx+1,0:ny+1), invhv(0:nx+1,0:ny+1)
      real frict(0:nx+1,0:ny+1)
c
      common /geom/ x, y, dx, dy, rx, ry, hy
      real x(0:nx+1), y(0:ny+1)
      real dx(0:nx), dy(0:ny)
      real rx(0:ny+1), ry, hy(0:ny+1)
c     
      common /mask/ umask, vmask, etamask
      real umask(0:nx+1,0:ny+1)
      real vmask(0:nx+1,0:ny+1)
      real etamask(0:nx+1,0:ny+1)
c     
      common /ini/ inidepth, uini, vini, etaini
      real inidepth(nx,ny)
      real uini(nx,ny)
      real vini(nx,ny)
      real etaini(nx,ny)
c
      common /scales/ scaledepth, scaleu, scalev, scaleeta
      real scaledepth(nx,ny)
      real scaleu(nx,ny), scalev(nx,ny), scaleeta(nx,ny)


