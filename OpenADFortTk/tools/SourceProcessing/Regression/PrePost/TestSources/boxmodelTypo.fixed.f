      program boxmodelTypo
         integer :: ndim
         real :: proj_t

         ndim = 10
         do l = 1, 2*ndim
            if ( l. LE. ndim ) then
               proj_t = 1.D0
            else
               proj_t = 2.D0
            endif
         enddo
      end program boxmodelTypo
