      module force
      use size 
!c     forcing fields for momentum equations
!un      common /force/ uforce, vforce
        SAVE
      real uforce(nx,ny)
      real vforce(nx,ny)
!c
      end module
     
