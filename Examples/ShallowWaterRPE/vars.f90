      module vars
      use size
!c     dynamic variables velocity and sea-surface height
!un      common /vars/ u, v, eta
        SAVE
      real u(0:nx+1,0:ny+1)
      real v(0:nx+1,0:ny+1)
      real eta(0:nx+1,0:ny+1)
!c     
      end module
