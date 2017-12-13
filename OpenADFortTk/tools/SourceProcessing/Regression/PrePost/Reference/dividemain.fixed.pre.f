      module OAD_intrinsics
      end module
	   
	   
	   
       program main
       use OAD_intrinsics
       real,dimension(0:5) :: b = (/1.,2.,3.,4.,5.,6./)
       real,dimension(0:5) :: c = (/3.,0.,4.,0.,5.,6./)
       real, dimension (0:5) :: a
	   
          where (c/=0)
           a = b/c
          elsewhere
           a=0.
           end where
	   
         if (a(1).EQ.0.) then
             print *, "OK"
           else
             print *, "failed"
           end if
       end
