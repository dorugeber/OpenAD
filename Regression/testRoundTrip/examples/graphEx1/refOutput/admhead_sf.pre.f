!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      SUBROUTINE head ( x01, x02, x03, x04, x05, x06, x07, x08, x09, x10
     +, x11, x12, x13, x14, x15, x16, x17, x18, x19, y1, y2, y3, y4, y5)
      use OAD_intrinsics
      IMPLICIT none
      double precision,intent(in):: x01, x02, x03, x04, x05, x06, x07, x
     +08, x09, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19
      double precision,intent(out):: y1, y2, y3, y4, y5
      real:: one, half
      PARAMETER (one = 1.0d0, half = 0.5d0)
      double precision:: t01, t02, t03, t04, t05, t06, t07, t08, t09, t1
     +0, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23
     +, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35, t36,
     + t37, t38, t39, t40, t41, t42, t43, t44, t45, t46, t47, t48, t49, 
     +t50, t51, t52, t53, t54, t55, t56, t57, t58, t59, t60

C$openad INDEPENDENT(x01)
C$openad INDEPENDENT(x02)
C$openad INDEPENDENT(x03)
C$openad INDEPENDENT(x04)
C$openad INDEPENDENT(x05)
C$openad INDEPENDENT(x06)
C$openad INDEPENDENT(x07)
C$openad INDEPENDENT(x08)
C$openad INDEPENDENT(x09)
C$openad INDEPENDENT(x10)
C$openad INDEPENDENT(x11)
C$openad INDEPENDENT(x12)
C$openad INDEPENDENT(x13)
C$openad INDEPENDENT(x14)
C$openad INDEPENDENT(x15)
C$openad INDEPENDENT(x16)
C$openad INDEPENDENT(x17)
C$openad INDEPENDENT(x18)
C$openad INDEPENDENT(x19)

      t01 = sin (x01 **2 + x02 **2 + x03 **2)
      t02 = one/t01
      t03 = x01*t02
      t04 = x02*t02
      t05 = x03*t02
      t06 = one/(one+sin(x10/x05))
      t07 = one-t06
      t08 = half*(x06**2+x07**2+x08**2)
      t09 = half*(x11**2+x12**2+x13**2)
      t10 = (x14*x16)*x04/x05+t08
      t11 = (x14*x16)*x09/x10+t09
      t12 = t06*x06+t07*x11
      t13 = t06*x07+t07*x12
      t14 = t06*x08+t07*x13
      t15 = t06*t10+t07*t11
      t18 = half*(t12**2+t13**2+t14**2)
      t16 = x15*(t15-t18)
      t16 = sin (t16)
      t17 = t03*t12+t04*t13+t05*t14
      t19 = t04*t14-t05*t13
      t20 = t05*t12-t03*t14
      t21 = t03*t13-t04*t12
      t22 = t17-t16
      t24 = t17
      t23 = t17+t16
      t25 = t22
      t26 = t23
      t27 = t24
      t25 = 0.5d0*(t22-t25)
      t26 = 0.5d0*(t23-t26)
      t27 = 0.5d0*(t24-t27)
      t28 = x04*x16+x05*t08
      t29 = x05*x06
      t30 = x05*x07
      t31 = x05*x08
      t32 = x09*x16+x10*t09
      t33 = x10*x11
      t34 = x10*x12
      t35 = x10*x13
      t36 = t32-t28
      t37 = x10-x05
      t38 = t33-t29
      t39 = t34-t30
      t40 = t35-t31
      t46 = t12*t38+t13*t39+t14*t40
      t47 = (x15/t16)*(t36+t18*t37-t46)
      t48 = t03*t38+t04*t39+t05*t40
      t41 = half*(t47+t17*t37-t48)
      t42 = (-t03)*t47+(t03*t16-t19)*t37-t05*t39+t04*t40
      t43 = (-t04)*t47+(t04*t16-t20)*t37-t03*t40+t05*t38
      t44 = (-t05)*t47+(t05*t16-t21)*t37-t04*t38+t03*t39
      t45 = half*(t47-t17*t37+t48)
      t41 = t25*t41
      t42 = t27*t42
      t43 = t27*t43
      t44 = t27*t44
      t45 = t26*t45
      t49 = t41+t45
      t50 = t41-t45
      t56 = t49/t16+t03*t42+t04*t43+t05*t44
      t51 = t56*t18+(t16*x16*t49-t17*t50)+t19*t42+t20*t43+t21*t44
      t52 = t56
      t53 = t56*t12-t03*t50+t05*t43-t04*t44
      t54 = t56*t13-t04*t50+t03*t44-t05*t42
      t55 = t56*t14-t05*t50+t04*t42-t03*t43
      t57 = t03*x06+t04*x07+t05*x08
      y1 = t01*((t28+x04)*t57+t51)
      y2 = t01*(x05*t57+t52)
      y3 = t01*(t29*t57+t03*x04+t53)
      y4 = t01*(t30*t57+t04*x04+t54)
      y5 = t01*(t31*t57+t05*x04+t55)

C$openad DEPENDENT(y1)
C$openad DEPENDENT(y2)
C$openad DEPENDENT(y3)
C$openad DEPENDENT(y4)
C$openad DEPENDENT(y5)


      END SUBROUTINE
