	module m 
	  double precision, allocatable :: t(:)
	end module

	subroutine foo(t)
c$openad XXX Template ad_template.f
	  double precision :: t(:)
	  print *,shape(t)
	  call bar()
	  print *,shape(t)
	end subroutine

	subroutine bar()
c$openad XXX Template ad_template.f
	  use m
	  deallocate(t)
          allocate(t(3))
          do i=1,3
             t(i)=1.0D0+i*.5D0
	  end do
	end subroutine

	subroutine head(x,y) 
c$openad XXX Template ad_template.f
	  use m 
	  interface 
	     subroutine foo(t)
	     double precision :: t(:)
	     end subroutine
	  end interface
	  double precision, dimension(1) :: x,y
c$openad INDEPENDENT(x)
	  allocate(t(2))
          t(2)=x(1)
          t(2)=t(2)*2
          x(1)=t(2)
	  call foo(t)
	  y(1)=x(1)*t(3)
	  deallocate(t)
c$openad DEPENDENT(y)
	end subroutine

