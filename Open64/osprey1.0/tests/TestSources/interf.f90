program mytest
implicit none
integer, parameter :: dp=selected_real_kind(15, 60)

real(dp) :: p_db, pout_db

interface
  subroutine sub1 (p, pout)
    implicit none
	integer, parameter :: dp=selected_real_kind(15, 60)
    real(dp),intent(in) :: p
	real(dp),intent(out) :: pout
  end subroutine sub1
end interface

   p_db=.123456789
   call sub1(p_db, pout_db)
   if(pout_db.EQ.p_db) then
      print *, "OK"
   else
      print *, "failed"
   end if
end program mytest

subroutine sub1 (p, pout)
implicit none
integer, parameter :: dp=selected_real_kind(15, 60)
real(dp), intent(in) :: p
real(dp), intent(out) :: pout

pout=p

end subroutine sub1
