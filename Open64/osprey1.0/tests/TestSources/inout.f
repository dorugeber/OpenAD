! from http://www.cs.rpi.edu/~szymansk/oof90.html
		program intent_gotcha
         type mytype
           integer :: x
           real :: y
         end type mytype

         type (mytype) :: a
         a%x = 1 ; a%y = 2.
         call assign(a)
! a%y COULD BE UNDEFINED HERE IF NOT SET IN ASSIGN
         if(a%x.EQ.2) then
		   print *, "OK"
		 else
		   print *, "failed"
         end if
		 
         contains

         subroutine assign(this)
         type (mytype), intent (out) :: this
! THIS IS THE RIGHT WAY
         this%x = 2 ; this%y = 2.
         end subroutine assign
         end program intent_gotcha