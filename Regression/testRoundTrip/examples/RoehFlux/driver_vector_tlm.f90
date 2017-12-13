program driver

  use OAD_active
  implicit none 

  external ad_roehf5

  ! inputs
  type(active) nrm(3),priml(5),primr(5) 
  type(active) nrm_ph(3),priml_ph(5),primr_ph(5)
  type(active) gamma,gm1,gm1inv,nlefix,lefix,mcheps
  type(active) gamma_ph,gm1_ph,gm1inv_ph,nlefix_ph,lefix_ph,mcheps_ph

  ! outputs
  type(active) flux(5),flux_ph(5)

  double precision,dimension(19) :: x0
  double precision :: h
  integer :: n,m
  integer :: i,j

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n,m,h
  close(2)

  do i=1,n   
     x0(i)=sqrt(1.*i)
  end do

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  nrm(1)%v=x0(1)
  nrm(2)%v=x0(2)
  nrm(3)%v=x0(3)
  priml(1)%v=x0(4)
  priml(2)%v=x0(5)
  priml(3)%v=x0(6)
  priml(4)%v=x0(7)
  priml(5)%v=x0(8)
  primr(1)%v=x0(9)
  primr(2)%v=x0(10)
  primr(3)%v=x0(11)
  primr(4)%v=x0(12)
  primr(5)%v=x0(13)
  gamma%v=x0(14) 
  gm1%v=x0(15) 
  gm1inv%v=x0(16) 
  nlefix%v=x0(17)
  lefix%v=x0(18)
  mcheps%v=x0(19)

  call ad_roehf5 (nrm, priml, primr, gamma, gm1, gm1inv, nlefix, lefix, mcheps, flux)

  do i=1,n   
     nrm_ph(1)%v=x0(1)
     nrm_ph(2)%v=x0(2)
     nrm_ph(3)%v=x0(3)
     priml_ph(1)%v=x0(4)
     priml_ph(2)%v=x0(5)
     priml_ph(3)%v=x0(6)
     priml_ph(4)%v=x0(7)
     priml_ph(5)%v=x0(8)
     primr_ph(1)%v=x0(9)
     primr_ph(2)%v=x0(10)
     primr_ph(3)%v=x0(11)
     primr_ph(4)%v=x0(12)
     primr_ph(5)%v=x0(13)
     gamma_ph%v=x0(14) 
     gm1_ph%v=x0(15) 
     gm1inv_ph%v=x0(16) 
     nlefix_ph%v=x0(17)
     lefix_ph%v=x0(18)
     mcheps_ph%v=x0(19)

     if (i==1) nrm_ph(1)%v=nrm_ph(1)%v+h
     if (i==2) nrm_ph(2)%v=nrm_ph(2)%v+h
     if (i==3) nrm_ph(3)%v=nrm_ph(3)%v+h
     if (i==4) priml_ph(1)%v=priml_ph(1)%v+h
     if (i==5) priml_ph(2)%v=priml_ph(2)%v+h
     if (i==6) priml_ph(3)%v=priml_ph(3)%v+h
     if (i==7) priml_ph(4)%v=priml_ph(4)%v+h
     if (i==8) priml_ph(5)%v=priml_ph(5)%v+h
     if (i==9) primr_ph(1)%v=primr_ph(1)%v+h
     if (i==10) primr_ph(2)%v=primr_ph(2)%v+h
     if (i==11) primr_ph(3)%v=primr_ph(3)%v+h
     if (i==12) primr_ph(4)%v=primr_ph(4)%v+h
     if (i==13) primr_ph(5)%v=primr_ph(5)%v+h
     if (i==14) gamma_ph%v=gamma_ph%v+h
     if (i==15) gm1_ph%v=gm1_ph%v+h
     if (i==16) gm1inv_ph%v=gm1inv_ph%v+h
     if (i==17) nlefix_ph%v=nlefix_ph%v+h
     if (i==18) lefix_ph%v=lefix_ph%v+h
     if (i==19) mcheps_ph%v=mcheps_ph%v+h
     call ad_roehf5(nrm_ph, priml_ph, primr_ph, gamma_ph, gm1_ph, gm1inv_ph, nlefix_ph, lefix_ph, mcheps_ph, flux_ph)
     do j=1,m
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",j,",",i,")=",(flux_ph(j)%v-flux(j)%v)/h
     end do
  end do
  close(2)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  nrm(1)%v = x0(1)
  nrm(2)%v = x0(2)
  nrm(3)%v = x0(3)
  priml(1)%v = x0(4)
  priml(2)%v = x0(5)
  priml(3)%v = x0(6)
  priml(4)%v = x0(7)
  priml(5)%v = x0(8)
  primr(1)%v = x0(9)
  primr(2)%v = x0(10)
  primr(3)%v = x0(11)
  primr(4)%v = x0(12)
  primr(5)%v = x0(13)
  gamma%v = x0(14) 
  gm1%v = x0(15) 
  gm1inv%v = x0(16) 
  nlefix%v = x0(17)
  lefix%v = x0(18)
  mcheps%v = x0(19)

  do i=1,n
     nrm(1)%d(i) = 0.0D0
     nrm(2)%d(i) = 0.0D0
     nrm(3)%d(i) = 0.0D0
     priml(1)%d(i) = 0.0D0
     priml(2)%d(i) = 0.0D0
     priml(3)%d(i) = 0.0D0
     priml(4)%d(i) = 0.0D0
     priml(5)%d(i) = 0.0D0
     primr(1)%d(i) = 0.0D0
     primr(2)%d(i) = 0.0D0
     primr(3)%d(i) = 0.0D0
     primr(4)%d(i) = 0.0D0
     primr(5)%d(i) = 0.0D0
     gamma%d(i) = 0.0D0
     gm1%d(i) = 0.0D0
     gm1inv%d(i) = 0.0D0
     nlefix%d(i) = 0.0D0
     lefix%d(i) = 0.0D0
     mcheps%d(i) = 0.0D0
  end do
  nrm(1)%d(1) = 1.0D0
  nrm(2)%d(2) = 1.0D0
  nrm(3)%d(3) = 1.0D0
  priml(1)%d(4) = 1.0D0
  priml(2)%d(5) = 1.0D0
  priml(3)%d(6) = 1.0D0
  priml(4)%d(7) = 1.0D0
  priml(5)%d(8) = 1.0D0
  primr(1)%d(9) = 1.0D0
  primr(2)%d(10) = 1.0D0
  primr(3)%d(11) = 1.0D0
  primr(4)%d(12) = 1.0D0
  primr(5)%d(13) = 1.0D0
  gamma%d(14) = 1.0D0
  gm1%d(15) = 1.0D0
  gm1inv%d(16) = 1.0D0
  nlefix%d(17) = 1.0D0
  lefix%d(18) = 1.0D0
  mcheps%d(19) = 1.0D0
  call ad_roehf5(nrm,priml,primr,gamma,gm1,gm1inv,nlefix,lefix,mcheps,flux)
  do i=1,n
     do j=1,m
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",j,",",i,")=",flux(j)%d(i)
     end do
  end do
  close(2)

end program driver

