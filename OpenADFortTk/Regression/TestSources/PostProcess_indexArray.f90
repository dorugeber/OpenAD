program indexArray
  double precision:: l(2),r(3)
  integer ::  i(2)
!$openad INDEPENDENT(r)
  r(1)=1.1D0
  r(2)=1.2D0
  r(3)=1.3D0
  i(1)=1
  i(2)=3
!$openad DEPENDENT(l)
  l(:)=dble(r(i(1:)))
  print *,l
end program
