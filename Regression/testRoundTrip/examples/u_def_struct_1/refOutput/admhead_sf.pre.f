!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
      module myTypeModule

        implicit none
        private
        public :: myType
	
        type myType
      sequence
      double precision :: field1
      double precision :: field2=0.0
      end type myType
      end module

C$openad XXX Template ad_template.f
      subroutine head(x,y)
        use OAD_intrinsics
	
        use myTypeModule

        double precision, dimension(2) :: x
        double precision, dimension(1) :: y
        type(myType) :: typed_x
C$openad INDEPENDENT(x)
        typed_x%field1 = x(1)
        typed_x%field2 = x(2)
          y(1) = typed_x%field1*typed_x%field2
C$openad DEPENDENT(y)
      end subroutine
