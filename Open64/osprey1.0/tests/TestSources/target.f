! from  http://www.cs.rpi.edu/~szymansk/oof90.html

         module assign_pointer_class
         type mytype
            private
            integer, pointer :: pr
         end type mytype
         interface assignment (=)
            module procedure assign_pointer
         end interface
         contains
         subroutine assign_pointer(this, a)
         type (mytype), intent(out) :: this
         integer, target, intent(in) :: a
         this%pr => a
         end subroutine assign_pointer
		 
		 integer function get_val(this)
         type (mytype), intent(out) :: this
		 get_val=this%pr
		 end function get_val
		 
         end module assign_pointer_class
			
		 program main
         use assign_pointer_class
         type (mytype) :: x
! THIS IS THE RIGHT WAY
         integer, target :: a = 1
         call assign_pointer(x,a)
		 a=a+get_val(x);
		 if(a.EQ.2) then
                 print *, "OK"
                 else
                 print *, "failed"
                 end if
		 
         end program main
