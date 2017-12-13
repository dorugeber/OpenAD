c$openad XXX Template ad_template.f
	subroutine head(x,obj)
	  implicit none
	  double precision, dimension(1) :: obj
	  double precision, dimension(6) :: x

	  double precision f,g,loc1,d00,d11,d12,d03
	  double precision, dimension(0:3) :: matr
	  double precision m00,m03,m11,m12,m22,m33

	  double precision a,sqrt3 
c$openad INDEPENDENT(x)
	  a=5.00000000000000000000000000000d-01
	  sqrt3=5.77350269189625797959429519858d-01

	  matr(0)=x(2)-x(1)
	  matr(1)=(2.0*x(3)-x(2)-x(1))*sqrt3
	  matr(2)=x(5)-x(4)
	  matr(3)=(2.0*x(6)-x(5)-x(4))*sqrt3

	  m03=matr(0)*matr(3)
	  m12=matr(1)*matr(2)
	  g=m03-m12
	  d00=2.*m03
	  d11=2.*m12

	  m00=matr(0)*matr(0)
	  m11=matr(1)*matr(1)
	  m22=matr(2)*matr(2)
	  m33=matr(3)*matr(3)
 
	  d12=m00*m33
	  d03=m11*m22

	  f=d12+d03

	  loc1=a*(1/g)
	  obj(1)=f*loc1
c$openad DEPENDENT(obj)
	end subroutine
