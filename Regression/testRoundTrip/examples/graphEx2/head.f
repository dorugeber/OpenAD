c$openad XXX Template ad_template.f
      SUBROUTINE head (
     &     x01, x02, x03, x04, x05, x06,
     &     y01, y02, y03, y04, y05, y06, y07, y08, y09, y10, y11, y12, 
     &     y13, y14, y15, y16, y17, y18, y19, y20, y21, y22, y23, y24,                                 
     &     y25, y26, y27, y28
     &     ) 
      IMPLICIT none               
      double precision,intent(in):: 
     &     x01, x02, x03, x04, x05, x06 
      double precision,intent(out):: 
     &     y01, y02, y03, y04, y05, y06, y07, y08, y09, y10, y11, y12, 
     &     y13, y14, y15, y16, y17, y18, y19, y20, y21, y22, y23, y24,                                 
     &     y25, y26, y27, y28

      double precision::obj
      double precision::g_obj0
      double precision::g_obj1
      double precision::g_obj2
      double precision::g_obj3
      double precision::g_obj4
      double precision::g_obj5
      double precision::h_obj0
      double precision::h_obj1
      double precision::h_obj2
      double precision::h_obj3
      double precision::h_obj4
      double precision::h_obj5
      double precision::h_obj6
      double precision::h_obj7
      double precision::h_obj8
      double precision::h_obj9
      double precision::h_obj10
      double precision::h_obj11
      double precision::h_obj12
      double precision::h_obj13
      double precision::h_obj14
      double precision::h_obj15
      double precision::h_obj16
      double precision::h_obj17
      double precision::h_obj18
      double precision::h_obj19
      double precision::h_obj20

      double precision f,g,loc1,loc2,gg,d00,d11,d12,d03,d01
      double precision matr0,matr1,matr2,matr3,adj_m0,adj_m1,adj_m2,
     &adj_m3
      double precision m00,m01,m02,m03,m11,m12,m13,m22,m23,m33
      double precision r00,r01,r02,r03,r11,r12,r13,r22,r23,r33
  
      double precision ab,twosqrt3,third,two3rd,four3rd
      double precision a,b,sqrt3,tsqrt3,bm1

c$openad INDEPENDENT(x01)
c$openad INDEPENDENT(x02)
c$openad INDEPENDENT(x03)
c$openad INDEPENDENT(x04)
c$openad INDEPENDENT(x05)
c$openad INDEPENDENT(x06)

      a=5.00000000000000000000000000000e-01
      b=-1.00000000000000000000000000000e-00
      ab=a*b
      sqrt3=5.77350269189625797959429519858e-01
      tsqrt3=1.15470053837925159591885903972e+00
      twosqrt3=1.15470053837925159591885903972e+00
      third=.33333333333333333333333e+00
      two3rd=.66666666666666666666666e+00
      four3rd=1.33333333333333333333333e+00
      bm1=-2.00000000000000000000000000000e-00
 
      matr0=x02-x01
      matr1=(2.0*x03-x02-x01)*sqrt3
      matr2=x05-x04
      matr3=(2.0*x06-x05-x04)*sqrt3

      m03=matr0*matr3
      m12=matr1*matr2
      g=m03-m12
      d00=2.*m03
      d11=2.*m12

      m00=matr0*matr0
      m11=matr1*matr1
      m22=matr2*matr2
      m33=matr3*matr3
 
      d12=m00*m33
      d03=m11*m22

      f=d12+d03

      loc1=a*(1/g)
      obj=f*loc1

      loc2=ab*(1/(g*g))
      gg=ab*b*(1/(g*g*g))

      g=f*loc2
      f=2.*loc1

      adj_m0=matr0*f+matr3*g
      adj_m1=matr1*f-matr2*g
      adj_m2=matr2*f-matr1*g
      adj_m3=matr3*f+matr0*g

      loc1=sqrt3*adj_m1
      g_obj0=-adj_m0-loc1 
      g_obj1=adj_m0-loc1
      g_obj2=2.0*loc1
      loc1=sqrt3*adj_m3
      g_obj3=-adj_m2-loc1
      g_obj4=adj_m2-loc1
      g_obj5=2.0*loc1

      loc2=loc2*2.
      r00=loc2*d00+m33*gg
      r11=loc2*d11+m22*gg
      r22=loc2*d11+m11*gg
      r33=loc2*d00+m00*gg

      m01=matr0*matr1
      m02=matr0*matr2
      m12=matr1*matr2
      m13=matr1*matr3
      m23=matr2*matr3
      d01=loc2*(m02+m13)

      loc1=third*r11
      f=twosqrt3*(d01+m23*gg)  
      g=loc1+r00
      h_obj1=loc1-r00 	 
      h_obj0=g+f		 
      h_obj3=g-f		 
      loc1=-two3rd*r11
      h_obj2=loc1-f	
      h_obj4=loc1+f	
      h_obj5=four3rd*r11	

      loc1= third*r00;
      f=twosqrt3*(d01+m01*gg) 
      g=loc1+r11
      h_obj16= loc1 - r11	
      h_obj15=g+f		
      h_obj18=g-f	
      loc1=-two3rd*r00
      h_obj17=loc1-f
      h_obj19=loc1+f
      h_obj20=four3rd*r00	

      loc1=loc2*(m01+m23)   

      r13=loc1+gg*m02
      r02=loc1+gg*m13

      r03=loc2*d03+gg*m03
      r12=loc2*d12+gg*m12

      g=-two3rd*r13
      f=twosqrt3*r03
      h_obj8=g-f	
      h_obj11=g+f
      f=twosqrt3*r12
      h_obj12=g-f 	
      h_obj13=g+f 	
      h_obj14=12.*r13
      g=four3rd*r02;
      f=sqrt3*(r03+r12)
      h_obj10=g-f 
      h_obj6=g+f  
      g=two3rd*r02
      f=sqrt3*(r03-r12)
      h_obj9=g-f	
      h_obj7=g+f


C second time around

      matr0=g_obj1-g_obj0
      matr1=(2.0*g_obj2-g_obj1-g_obj2)*sqrt3
      matr2=g_obj4-g_obj3
      matr3=(2.0*g_obj5-g_obj4-g_obj3)*sqrt3

      m03=matr0*matr3
      m12=matr1*matr2
      g=m03-m12
      d00=2.*m03
      d11=2.*m12

      m00=matr0*matr0
      m11=matr1*matr1
      m22=matr2*matr2
      m33=matr3*matr3
 
      d12=m00*m33
      d03=m11*m22

      f=d12+d03

      loc1=a*(1/g)
      obj=f*loc1

      loc2=ab*(1/(g*g))
      gg=ab*b*(1/(g*g*g))

      g=f*loc2
      f=2.*loc1

      adj_m0=matr0*f+matr3*g
      adj_m1=matr1*f-matr2*g
      adj_m2=matr2*f-matr1*g
      adj_m3=matr3*f+matr0*g

      loc1=sqrt3*adj_m1
      g_obj0=g_obj0-adj_m0-loc1 
      g_obj1=g_obj1+adj_m0-loc1
      g_obj2=g_obj1+2.0*loc1
      loc1=sqrt3*adj_m3
      g_obj3=g_obj3-adj_m2-loc1
      g_obj4=g_obj4+adj_m2-loc1
      g_obj5=g_obj5+2.0*loc1

      loc2=loc2*2.
      r00=loc2*d00+m33*gg
      r11=loc2*d11+m22*gg
      r22=loc2*d11+m11*gg
      r33=loc2*d00+m00*gg

      m01=matr0*matr1
      m02=matr0*matr2
      m12=matr1*matr2
      m13=matr1*matr3
      m23=matr2*matr3
      d01=loc2*(m02+m13)

      loc1=third*r11
      f=twosqrt3*(d01+m23*gg)  
      g=loc1+r00
      h_obj1=h_obj1+loc1-r00 	 
      h_obj0=h_obj0+g+f		 
      h_obj3=h_obj3+g-f		 
      loc1=-two3rd*r11
      h_obj2=h_obj2+loc1-f	
      h_obj4=h_obj4+loc1+f	
      h_obj5=h_obj5+four3rd*r11	

      loc1= third*r00;
      f=twosqrt3*(d01+m01*gg) 
      g=loc1+r11
      h_obj16= h_obj16+loc1 - r11	
      h_obj15=h_obj15+g+f		
      h_obj18=h_obj18+g-f	
      loc1=-two3rd*r00
      h_obj17=h_obj17+loc1-f
      h_obj19=h_obj19+loc1+f
      h_obj20=h_obj20+four3rd*r00	

      loc1=loc2*(m01+m23)   

      r13=loc1+gg*m02
      r02=loc1+gg*m13

      r03=loc2*d03+gg*m03
      r12=loc2*d12+gg*m12

      g=-two3rd*r13
      f=twosqrt3*r03
      h_obj8=h_obj8+g-f	
      h_obj11=h_obj11+g+f
      f=twosqrt3*r12
      h_obj12=h_obj12+g-f 	
      h_obj13=h_obj13+g+f 	
      h_obj14=h_obj14+12.*r13
      g=four3rd*r02;
      f=sqrt3*(r03+r12)
      h_obj10=h_obj10+g-f 
      h_obj6=h_obj6+g+f  
      g=two3rd*r02
      f=sqrt3*(r03-r12)
      h_obj9=h_obj9+g-f	
      h_obj7=h_obj7+g+f
      y01=obj
      y02=g_obj0
      y03=g_obj1
      y04=g_obj2
      y05=g_obj3
      y06=g_obj4
      y07=g_obj5
      y08=h_obj0
      y09=h_obj1
      y10=h_obj2
      y11=h_obj3
      y12=h_obj4
      y13=h_obj5
      y14=h_obj6
      y15=h_obj7
      y16=h_obj8
      y17=h_obj9
      y18=h_obj10
      y19=h_obj11
      y20=h_obj12
      y21=h_obj13
      y22=h_obj14
      y23=h_obj15
      y24=h_obj16
      y25=h_obj17
      y26=h_obj18
      y27=h_obj19
      y28=h_obj20

c$openad DEPENDENT(y01)
c$openad DEPENDENT(y02)
c$openad DEPENDENT(y03)
c$openad DEPENDENT(y04)
c$openad DEPENDENT(y05)                                                                        
c$openad DEPENDENT(y06)
c$openad DEPENDENT(y07)
c$openad DEPENDENT(y08)
c$openad DEPENDENT(y09)
c$openad DEPENDENT(y10)                                                                        
c$openad DEPENDENT(y11)
c$openad DEPENDENT(y12)
c$openad DEPENDENT(y13)
c$openad DEPENDENT(y14)
c$openad DEPENDENT(y15)                                                                        
c$openad DEPENDENT(y16)
c$openad DEPENDENT(y17)
c$openad DEPENDENT(y18)
c$openad DEPENDENT(y19)
c$openad DEPENDENT(y20)                                                                        
c$openad DEPENDENT(y21)
c$openad DEPENDENT(y22)
c$openad DEPENDENT(y23)
c$openad DEPENDENT(y24)
c$openad DEPENDENT(y25)                                                                        
c$openad DEPENDENT(y26)
c$openad DEPENDENT(y27)
c$openad DEPENDENT(y28)

      END SUBROUTINE 
