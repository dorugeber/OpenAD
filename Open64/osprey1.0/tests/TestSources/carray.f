        module carray
          interface countarray
             module procedure c1, c2, cn
          end interface
          contains
          subroutine c1(f,cs)
            real, dimension(:), intent(in) :: f
            integer, intent(out) :: cs

            cs=size(f)
          end subroutine c1

          subroutine c2(f,cs)
            real, dimension(:,:), intent(in) :: f
            integer, intent(out) :: cs

            cs=size(f)
          end subroutine c2

          subroutine cn(f,cs)
            real, intent(in) :: f
            integer, intent(out) :: cs

            cs=0
          end subroutine cn
        end module carray

        program mc
         use carray
         real  :: g0
         real, dimension (3) :: g1
         real, dimension (2,2) :: g2
         integer :: sn, s1, s2
         g0=0.
         g1=(/1., 2., 3./)
         g2=reshape((/1., 2., 3., 4./),(/2,2/))
         call countarray(g0, sn)
         call countarray(g1, s1)
         call countarray(g2, s2)
         if((sn.EQ.0).AND.(s1.EQ.3).AND.(s2.EQ.4)) then
           print *, "OK"
         else
           print *, "failed"
         end if
        end program mc  
