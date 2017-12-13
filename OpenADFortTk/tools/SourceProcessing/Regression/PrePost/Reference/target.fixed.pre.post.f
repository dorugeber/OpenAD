module OAD_intrinsics
end module
! from  http://www.cs.rpi.edu/~szymansk/oof90.html

   module assign_pointer_class
   use OAD_active
   use OAD_intrinsics
   type mytype
      private
      integer, pointer :: pr
   end type mytype
   interface assignment (=)
      module procedure assign_pointer
   end interface
   contains
   subroutine assign_pointer(this, a)
     type(mytype),intent(out) :: this
     integer, target, intent(in) :: a
     this%pr => a
   end subroutine assign_pointer
   integer function get_val(this)
      type(mytype),intent(in) :: this
      get_val=this%pr
   end function get_val
   subroutine oad_s_get_val(this,get_val)
      type(mytype),intent(in) :: this
      integer,intent(out) :: get_val
      get_val=this%pr
   end subroutine oad_s_get_val
   end module assign_pointer_class
   program main
     use OAD_active
     use OAD_intrinsics
     use assign_pointer_class
     type(mytype) :: x
! THIS IS THE RIGHT WAY
     integer, target :: a = 1
     integer :: oad_ctmp0
     call assign_pointer(x,a)
     call oad_s_get_val(x,oad_ctmp0)
     a = a+oad_ctmp0
     if (a.EQ.2) then
        print *,"OK"
     else
        print *,"failed"
     end if
   end program main
