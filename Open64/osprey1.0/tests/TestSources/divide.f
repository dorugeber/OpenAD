       subroutine divide (myb, myc, mya)
       real myb(6)
       real myc(6)
       real mya(6)
          where (myc/=0)
           mya=myb/myc
          elsewhere
           mya=0.
           end where
          end
	   
	   
	   
       program main
       real, dimension (0:5) :: b=(/1. , 2., 3., 4., 5., 6./)
       real, dimension (0:5) :: c=(/3., 0., 4., 0., 5., 6./)
       real, dimension (0:5) :: a
       call	 divide(b, c, a)
	   if(a(1).EQ.0.) then
             print *, "OK"
           else
             print *, "failed"
           end if
       end
