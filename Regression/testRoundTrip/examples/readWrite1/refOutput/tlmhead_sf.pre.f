!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
        double precision, dimension(1) :: x
        double precision, dimension(1) :: y
C$openad INDEPENDENT(x)
        open(3,file='data.tmp')
        write(3,'(EN26.16E3)') x(1)
        close(3)
        open(3,file='data.tmp')
        read(3,'(EN26.16E3)') x(1)
        close(3)
          y(1)=x(1)
C$openad DEPENDENT(y)
      end subroutine
