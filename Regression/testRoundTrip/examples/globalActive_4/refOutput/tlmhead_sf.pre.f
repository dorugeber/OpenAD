!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod
        double precision x(2)
        double precision y(1)
      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
       subroutine foo()
         use OAD_intrinsics

         use all_globals_mod

         y(1) = x(1)*x(2)

       end subroutine

C$openad XXX Template ad_template.f
       subroutine head()
         use OAD_intrinsics

         use all_globals_mod

C$openad INDEPENDENT(x)
         call foo()
C$openad DEPENDENT(y)

       end subroutine
