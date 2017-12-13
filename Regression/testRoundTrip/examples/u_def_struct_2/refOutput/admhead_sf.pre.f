	module myTypeModule

        implicit none
        private
        public :: myType
	
        type myType
	sequence
	double precision :: field1 
	double precision :: field2=0.0D0
	end type myType
	end module

c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	
	  use myTypeModule

	  double precision, dimension(4) :: x
	  double precision, dimension(1) :: y
	  type(myType) , dimension(2) :: typed_x
	  integer i
c$openad INDEPENDENT(x)
	  do i=1,2
	     typed_x(i)%field1 = x((i-1)*2+1.0D0)
	     typed_x(i)%field2 = x((i-1)*2+2.0D0)
	  end do
	  y(1)=.0D0
	  do i=1,2
	     y(1) = y(1)+typed_x(i)%field1 * typed_x(i)%field2
	  end do
c$openad DEPENDENT(y)
	end subroutine
