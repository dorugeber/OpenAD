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

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	
	  use myTypeModule

	  double precision, dimension(2) :: x
	  double precision, dimension(1) :: y
	  type(myType) :: typed_x
c$openad INDEPENDENT(x)
	  typed_x%field1 = x(1)
	  typed_x%field2 = x(2)
          y(1) = typed_x%field1 * typed_x%field2
c$openad DEPENDENT(y)
	end subroutine
