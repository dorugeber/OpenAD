!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine head(obj,g_obj0,g_obj1,g_obj2,g_obj3,g_obj4,g_obj5,x0,x
     +1,x2,x3,x4,x5,h_obj0,h_obj1,h_obj2,h_obj3,h_obj4,h_obj5,h_obj6,h_o
     +bj7,h_obj8,h_obj9,h_obj10,h_obj11,h_obj12,h_obj13,h_obj14,h_obj15,
     +h_obj16,h_obj17,h_obj18,h_obj19,h_obj20)
      use OAD_intrinsics

      implicit none
      double precision obj,g_obj0,g_obj1,g_obj2,g_obj3,g_obj4,g_obj5
      double precision x0,x1,x2,x3,x4,x5
      double precision h_obj0,h_obj1,h_obj2,h_obj3,h_obj4,h_obj5,h_obj6,
     +h_obj7,h_obj8,h_obj9,h_obj10,h_obj11,h_obj12,h_obj13,h_obj14,h_obj
     +15,h_obj16,h_obj17,h_obj18,h_obj19,h_obj20

      double precision f,g,loc1,loc2,norm,gg,d00,d11,d12,d03,d01
      double precision matr0,matr1,matr2,matr3,adj_m0,adj_m1,adj_m2,adj_
     +m3
      double precision m00,m01,m02,m03,m11,m12,m13,m22,m23,m33
      double precision r00,r01,r02,r03,r11,r12,r13,r22,r23,r33
  
      double precision ab,twosqrt3,third,two3rd,four3rd
      double precision a,b,sqrt3,tsqrt3,bm1

C$openad INDEPENDENT(x0)
C$openad INDEPENDENT(x1)
C$openad INDEPENDENT(x2)
C$openad INDEPENDENT(x3)
C$openad INDEPENDENT(x4)
C$openad INDEPENDENT(x5)

      a=5.00000000000000000000000000000d-01
      b = -1.00000000000000000000000000000d-00
      ab = a*b
      sqrt3=5.77350269189625797959429519858d-01
      tsqrt3=1.15470053837925159591885903972d+00
      twosqrt3=1.15470053837925159591885903972d+00
      third=.33333333333333333333333d+00
      two3rd=.66666666666666666666666d+00
      four3rd=1.33333333333333333333333d+00
      bm1 = -2.00000000000000000000000000000d-00
 
      matr0 = x1-x0
      matr1 = (2.0*x2-x1-x0)*sqrt3
      matr2 = x4-x3
      matr3 = (2.0*x5-x4-x3)*sqrt3

      m03 = matr0*matr3
      m12 = matr1*matr2
      g = m03-m12
      d00 = 2.*m03
      d11 = 2.*m12

      m00 = matr0*matr0
      m11 = matr1*matr1
      m22 = matr2*matr2
      m33 = matr3*matr3
 
      d12 = m00*m33
      d03 = m11*m22

      f = d12+d03
      norm=f

      loc1 = a*(1/g)
      obj = f*loc1

      loc2 = ab*(1/(g*g))
      gg = ab*b*(1/(g*g*g))

      g = f*loc2
      f = 2.*loc1

      adj_m0 = matr0*f+matr3*g
      adj_m1 = matr1*f-matr2*g
      adj_m2 = matr2*f-matr1*g
      adj_m3 = matr3*f+matr0*g

      loc1 = sqrt3*adj_m1
      g_obj0 = -adj_m0-loc1
      g_obj1 = adj_m0-loc1
      g_obj2 = 2.0*loc1
      loc1 = sqrt3*adj_m3
      g_obj3 = -adj_m2-loc1
      g_obj4 = adj_m2-loc1
      g_obj5 = 2.0*loc1

      loc2 = loc2*2.
      r00 = loc2*d00+m33*gg
      r11 = loc2*d11+m22*gg
      r22 = loc2*d11+m11*gg
      r33 = loc2*d00+m00*gg

      m01 = matr0*matr1
      m02 = matr0*matr2
      m12 = matr1*matr2
      m13 = matr1*matr3
      m23 = matr2*matr3
      d01 = loc2*(m02+m13)

      loc1 = third*r11
      f = twosqrt3*(d01+m23*gg)
      g = loc1+r00
      h_obj1 = loc1-r00
      h_obj0 = g+f
      h_obj3 = g-f
      loc1 = -two3rd*r11
      h_obj2 = loc1-f
      h_obj4 = loc1+f
      h_obj5 = four3rd*r11

      loc1 = third*r00
      f = twosqrt3*(d01+m01*gg)
      g = loc1+r11
      h_obj16 = loc1-r11
      h_obj15 = g+f
      h_obj18 = g-f
      loc1 = -two3rd*r00
      h_obj17 = loc1-f
      h_obj19 = loc1+f
      h_obj20 = four3rd*r00

      loc1 = loc2*(m01+m23)

      r13 = loc1+gg*m02
      r02 = loc1+gg*m13

      r03 = loc2*d03+gg*m03
      r12 = loc2*d12+gg*m12

      g = -two3rd*r13
      f = twosqrt3*r03
      h_obj8 = g-f
      h_obj11 = g+f
      f = twosqrt3*r12
      h_obj12 = g-f
      h_obj13 = g+f
      h_obj14 = 12.*r13
      g = four3rd*r02
      f = sqrt3*(r03+r12)
      h_obj10 = g-f
      h_obj6 = g+f
      g = two3rd*r02
      f = sqrt3*(r03-r12)
      h_obj9 = g-f
      h_obj7 = g+f

C$openad DEPENDENT(obj)
C$openad DEPENDENT(g_obj0)
C$openad DEPENDENT(g_obj1)
C$openad DEPENDENT(g_obj2)
C$openad DEPENDENT(g_obj3)
C$openad DEPENDENT(g_obj4)
C$openad DEPENDENT(g_obj5)
C$openad DEPENDENT(h_obj0)
C$openad DEPENDENT(h_obj1)
C$openad DEPENDENT(h_obj2)
C$openad DEPENDENT(h_obj3)
C$openad DEPENDENT(h_obj4)
C$openad DEPENDENT(h_obj5)
C$openad DEPENDENT(h_obj6)
C$openad DEPENDENT(h_obj7)
C$openad DEPENDENT(h_obj8)
C$openad DEPENDENT(h_obj9)
C$openad DEPENDENT(h_obj10)
C$openad DEPENDENT(h_obj11)
C$openad DEPENDENT(h_obj12)
C$openad DEPENDENT(h_obj13)
C$openad DEPENDENT(h_obj14)
C$openad DEPENDENT(h_obj15)
C$openad DEPENDENT(h_obj16)
C$openad DEPENDENT(h_obj17)
C$openad DEPENDENT(h_obj18)
C$openad DEPENDENT(h_obj19)
C$openad DEPENDENT(h_obj20)

      end subroutine
