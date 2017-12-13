c$openad XXX Template ad_template.f
	subroutine head(x,y) 
	  double precision, dimension(2) :: x
	  double precision, dimension(1) :: y
c$openad INDEPENDENT(x)
	  open(3,file='data.tmp')
	  write(3,'(EN26.16E3,EN26.16E3)') x
	  close(3)
	  open(3,file='data.tmp')
	  read(3,'(EN26.16E3,EN26.16E3)') x
	  close(3)
          y(1)=2*x(1)+3*x(2)
c$openad DEPENDENT(y)
	end subroutine
