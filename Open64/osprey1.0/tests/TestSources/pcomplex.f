      module private_complex_class
! define private_complex type
      type private_complex
      real :: real, imaginary
      end type private_complex
      interface operator(*)
         module procedure pc_mult
      end interface
      contains
         function pc_mult(a,b) result(c)
! multiply private_complex variables
         type (private_complex), intent(in) :: a, b
         type (private_complex) :: c
         c%real = a%real*b%real - a%imaginary*b%imaginary
         c%imaginary = a%real*b%imaginary + a%imaginary*b%real
         end function pc_mult
      end module private_complex_class
	  
      module monitor_complex_class
      use private_complex_class
      type monitor_complex
         type (private_complex) :: pc
         character*8 :: last_op
      end type monitor_complex
      interface operator(*)
         module procedure mc_mult
      end interface
      contains
         function mc_mult(a,b) result(c)
         type (monitor_complex), intent(in) :: a, b
         type (monitor_complex) :: c
         c%pc = pc_mult(a%pc,b%pc)
         c%last_op = 'MULTIPLY'
         end function mc_mult
      end module monitor_complex_class
	  
      program main
      use monitor_complex_class
      type (private_complex) :: a, b, c
      type (monitor_complex) :: x, y, z
      a%real=1.
      a%imaginary=0.
      b%real=1.
      b%imaginary=0.
      x%pc%real=1.
      x%pc%imaginary=0.
      y%pc%real=1.
      y%pc%imaginary=0.
      c = a*b   ! multiplication with private_complex types
      z = x*y   ! multiplication with monitor_complex types
      if((c%imaginary.EQ.0.).AND.(z%last_op.EQ.'MULTIPLY')) then
         print *, "OK"
      else
         print *, "failed"
      end if
      end program main	  
