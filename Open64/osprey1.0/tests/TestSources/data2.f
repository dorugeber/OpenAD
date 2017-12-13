        program mc
         real, dimension (2,2) :: g2
         g2=reshape((/1., 2., 3., 4./),(/2,2/))
         if((g2(2,1).EQ.2.0)) then
           print *, "OK"
         else
           print *, "failed"
         end if
        end program mc  
