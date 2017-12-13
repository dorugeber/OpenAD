c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(1) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
	  open(3,file='data.tmp')
	  write(3,'(EN26.16E3)') x(1)
	  close(3)
	  open(3,file='data.tmp')
	  read(3,'(EN26.16E3)') x(1)
	  close(3)
          y(1)=x(1)
c$openad DEPENDENT(y)
	end subroutine
