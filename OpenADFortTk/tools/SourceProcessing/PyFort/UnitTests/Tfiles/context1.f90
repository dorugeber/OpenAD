!
! subroutine header
!
subroutine s1(a,b,c)
   double precision a(10,5)
   character*4,dimension(2) :: b
   integer,dimension(10,3*5) :: c(3),d

   a(1,1) = real(b(1))
   d(1,1) = c(2)
end
