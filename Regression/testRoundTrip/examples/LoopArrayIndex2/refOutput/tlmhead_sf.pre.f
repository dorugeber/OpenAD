!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine bar(k,a,p)
       use OAD_intrinsics
       double precision a
       double precision p
       integer k
       a = a*p*k
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
       use OAD_intrinsics
       double precision x
       double precision y
       double precision :: p(2)
       integer i,j
C$openad INDEPENDENT(x)
C$openad xxx simple loop
       p(1)=3.0D0
       p(2)=6.0D0
       do j=1,2
         call bar(j,x ,p(j))
       enddo
       y = x
C$openad DEPENDENT(y)
      end subroutine

