program driver

  use OAD_active
  implicit none 

  external head

  type(active) obj,g_obj0,g_obj1,g_obj2,g_obj3,g_obj4,g_obj5
  type(active) x1,x2,x3,x4,x5,x6
  type(active) h_obj0,h_obj1,h_obj2,h_obj3,h_obj4,h_obj5,h_obj6,&
       &h_obj7,h_obj8,h_obj9,h_obj10,h_obj11,h_obj12,h_obj13,h_obj14,&
       &h_obj15,h_obj16,h_obj17,h_obj18,h_obj19,h_obj20

  type(active) objph,g_obj0ph,g_obj1ph,g_obj2ph,g_obj3ph,&
       &g_obj4ph,g_obj5ph,x1ph,x2ph,x3ph,x4ph,x5ph,x6ph,&
       &h_obj0ph,h_obj1ph,h_obj2ph,h_obj3ph,h_obj4ph,h_obj5ph,&
       &h_obj6ph,h_obj7ph,h_obj8ph,&
       &h_obj9ph,h_obj10ph,h_obj11ph,h_obj12ph,h_obj13ph,h_obj14ph,&
       &h_obj15ph,h_obj16ph,&
       &h_obj17ph,h_obj18ph,h_obj19ph,h_obj20ph

  double precision, dimension(:), allocatable :: res_dd
  double precision h
  integer n,m
  integer i,k

  open(2,action='read',file='params.conf')
  read(2,'(I5,/,I5,/,F8.1)') n,m,h
  close(2)

  allocate(res_dd(m))

  open(2,file='tmpOutput/dd.out')
  write(2,*) "DD"
  ! equilateral triangle coordinates (x1,x2,x3,y1,y2,y3)
  x1%v = 0.D0
  x2%v = 0.5D0
  x3%v = 1.D0
  x4%v = 0.D0
  x5%v = 0.523598776D0
  x6%v = 0.D0
  call head(obj,g_obj0,g_obj1,g_obj2,g_obj3,&
       &g_obj4,g_obj5,x1,x2,x3,x4,x5,x6,&
       &h_obj0,h_obj1,h_obj2,h_obj3,h_obj4,h_obj5,h_obj6,h_obj7,h_obj8,&
       &h_obj9,h_obj10,h_obj11,h_obj12,h_obj13,h_obj14,h_obj15,h_obj16,&
       &h_obj17,h_obj18,h_obj19,h_obj20)
  do i=1,n   
     x1ph%v=x1%v
     x2ph%v=x2%v
     x3ph%v=x3%v
     x4ph%v=x4%v
     x5ph%v=x5%v
     x6ph%v=x6%v
     if (i==1) x1ph%v=x1%v+h
     if (i==2) x2ph%v=x2%v+h
     if (i==3) x3ph%v=x3%v+h
     if (i==4) x4ph%v=x4%v+h
     if (i==5) x5ph%v=x5%v+h
     if (i==6) x6ph%v=x6%v+h
     call head(objph,g_obj0ph,g_obj1ph,g_obj2ph,g_obj3ph,&
          &g_obj4ph,g_obj5ph,x1ph,x2ph,x3ph,x4ph,x5ph,x6ph,&
          &h_obj0ph,h_obj1ph,h_obj2ph,h_obj3ph,h_obj4ph,h_obj5ph,&
          &h_obj6ph,h_obj7ph,h_obj8ph,&
          &h_obj9ph,h_obj10ph,h_obj11ph,h_obj12ph,h_obj13ph,h_obj14ph,&
          &h_obj15ph,h_obj16ph,&
          &h_obj17ph,h_obj18ph,h_obj19ph,h_obj20ph)
     res_dd(1) =(objph%v-obj%v)/h
     res_dd(2) =(g_obj0ph%v-g_obj0%v)/h
     res_dd(3) =(g_obj1ph%v-g_obj1%v)/h
     res_dd(4) =(g_obj2ph%v-g_obj2%v)/h
     res_dd(5) =(g_obj3ph%v-g_obj3%v)/h
     res_dd(6) =(g_obj4ph%v-g_obj4%v)/h
     res_dd(7) =(g_obj5ph%v-g_obj5%v)/h
     res_dd(8) =(h_obj0ph%v-h_obj0%v)/h
     res_dd(9)=(h_obj1ph%v-h_obj1%v)/h
     res_dd(10)=(h_obj2ph%v-h_obj2%v)/h
     res_dd(11)=(h_obj3ph%v-h_obj3%v)/h
     res_dd(12)=(h_obj4ph%v-h_obj4%v)/h
     res_dd(13)=(h_obj5ph%v-h_obj5%v)/h
     res_dd(14)=(h_obj6ph%v-h_obj6%v)/h
     res_dd(15)=(h_obj7ph%v-h_obj7%v)/h
     res_dd(16)=(h_obj8ph%v-h_obj8%v)/h
     res_dd(17)=(h_obj9ph%v-h_obj9%v)/h
     res_dd(18)=(h_obj10ph%v-h_obj10%v)/h
     res_dd(19)=(h_obj11ph%v-h_obj11%v)/h
     res_dd(20)=(h_obj12ph%v-h_obj12%v)/h
     res_dd(21)=(h_obj13ph%v-h_obj13%v)/h
     res_dd(22)=(h_obj14ph%v-h_obj14%v)/h
     res_dd(23)=(h_obj15ph%v-h_obj15%v)/h
     res_dd(24)=(h_obj16ph%v-h_obj16%v)/h
     res_dd(25)=(h_obj17ph%v-h_obj17%v)/h
     res_dd(26)=(h_obj18ph%v-h_obj18%v)/h
     res_dd(27)=(h_obj19ph%v-h_obj19%v)/h
     res_dd(28)=(h_obj20ph%v-h_obj20%v)/h
     do k=1,m
        write(2,'(A,I3,A,I3,A,EN26.16E3)') "F(",k,",",i,")=",res_dd(k)
     end do
  end do
  close(2)
  deallocate(res_dd)

  open(2,file='tmpOutput/ad.out')
  write(2,*) "AD"
  do i=1,n   
     x1%d(i) = 0.0D0
     x2%d(i) = 0.0D0
     x3%d(i) = 0.0D0
     x4%d(i) = 0.0D0
     x5%d(i) = 0.0D0
     x6%d(i) = 0.0D0
  end do
  x1%d(1) = 1.0D0
  x2%d(2) = 1.0D0
  x3%d(3) = 1.0D0
  x4%d(4) = 1.0D0
  x5%d(5) = 1.0D0
  x6%d(6) = 1.0D0
  call head(obj,g_obj0,g_obj1,g_obj2,g_obj3,&
        &g_obj4,g_obj5,x1,x2,x3,x4,x5,x6,&
        &h_obj0,h_obj1,h_obj2,h_obj3,h_obj4,h_obj5,h_obj6,h_obj7,h_obj8,&
        &h_obj9,h_obj10,h_obj11,h_obj12,h_obj13,h_obj14,h_obj15,h_obj16,&
        &h_obj17,h_obj18,h_obj19,h_obj20)
  do i=1,n
     write(2,'(A,I3,A,EN26.16E3)') "F(1,",i,")=",obj%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(2,",i,")=",g_obj0%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(3,",i,")=",g_obj1%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(4,",i,")=",g_obj2%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(5,",i,")=",g_obj3%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(6,",i,")=",g_obj4%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(7,",i,")=",g_obj5%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(8,",i,")=",h_obj0%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(9,",i,")=",h_obj1%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(10,",i,")=",h_obj2%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(11,",i,")=",h_obj3%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(12,",i,")=",h_obj4%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(13,",i,")=",h_obj5%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(14,",i,")=",h_obj6%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(15,",i,")=",h_obj7%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(16,",i,")=",h_obj8%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(17,",i,")=",h_obj9%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(18,",i,")=",h_obj10%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(19,",i,")=",h_obj11%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(20,",i,")=",h_obj12%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(21,",i,")=",h_obj13%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(22,",i,")=",h_obj14%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(23,",i,")=",h_obj15%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(24,",i,")=",h_obj16%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(25,",i,")=",h_obj17%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(26,",i,")=",h_obj18%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(27,",i,")=",h_obj19%d(i)
     write(2,'(A,I3,A,EN26.16E3)') "F(28,",i,")=",h_obj20%d(i)
  end do
  close(2)

end program driver

