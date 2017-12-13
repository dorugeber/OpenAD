
      module active1_module
      use w2f__types
      implicit none
      private
      public :: active1, saxpy, sax, setderiv, zero_deriv, convert_p2a_scalar,
     +convert_a2p_scalar, convert_p2a_vector, convert_a2p_vector

      type active1
      sequence
      double precision :: v 
      double precision :: d=0.0
      end type active1

      interface saxpy
      module procedure saxpy_a_a
      end interface
      
      interface setderiv
      module procedure setderiv_a_a
      end interface

      interface zero_deriv
      module procedure zero_deriv_a
      end interface
      
      interface sax
      module procedure sax_d_a_a, sax_i_a_a
      end interface

      interface convert_p2a_scalar
      module procedure convert_p2a_scalar_impl
      end interface
      interface convert_a2p_scalar
      module procedure convert_a2p_scalar_impl
      end interface
      interface convert_p2a_vector
      module procedure convert_p2a_vector_impl
      end interface
      interface convert_a2p_vector
      module procedure convert_a2p_vector_impl
      end interface

      contains
      
c$openad XXX Template ad_template.f
      subroutine saxpy_a_a(a,x,y)
      double precision, intent(in) :: a
      type(active1), intent(in) :: x
      type(active1), intent(inout) :: y
      
      y%d=y%d+x%d*a
      end subroutine saxpy_a_a
      
      
c$openad XXX Template ad_template.f
      subroutine sax_d_a_a(a,x,y)
      double precision, intent(in) :: a
      type(active1), intent(in) :: x
      type(active1), intent(inout) :: y
      
      y%d=x%d*a
      end subroutine sax_d_a_a

c$openad XXX Template ad_template.f
      subroutine sax_i_a_a(a,x,y)
      integer(kind=w2f__i8), intent(in) :: a
      type(active1), intent(in) :: x
      type(active1), intent(inout) :: y
      
      y%d=x%d*a
      end subroutine sax_i_a_a
      
      
c$openad XXX Template ad_template.f
      subroutine setderiv_a_a(y,x)
      type(active1), intent(inout) :: y
      type(active1), intent(in) :: x
      
      y%d=x%d
      end subroutine setderiv_a_a

c$openad XXX Template ad_template.f
      subroutine zero_deriv_a(x)
      type(active1), intent(inout) :: x

      x%d=0.0d0
      end subroutine zero_deriv_a

c$openad XXX Template ad_template.f
      subroutine convert_a2p_scalar_impl(convertTo, convertFrom)
      double precision, intent(inout) :: convertTo
      type(active1), intent(in) :: convertFrom
      convertTo=convertFrom%v
      end subroutine

c$openad XXX Template ad_template.f
      subroutine convert_p2a_scalar_impl(convertTo, convertFrom)
      double precision, intent(in) :: convertFrom
      type(active1), intent(inout) :: convertTo
      convertTo%v=convertFrom
      end subroutine 

c$openad XXX Template ad_template.f
      subroutine convert_a2p_vector_impl(convertTo, convertFrom)
      type(active1), dimension(:), intent(in) :: convertFrom
      double precision, dimension(:), intent(inout) :: convertTo
      integer i
      do i=lbound(convertFrom,1),ubound(convertFrom,1)
         convertTo(i)=convertFrom(i)%v
      end do
      end subroutine

c$openad XXX Template ad_template.f
      subroutine convert_p2a_vector_impl(convertTo, convertFrom)
      double precision, dimension(:), intent(in) :: convertFrom
      type(active1), dimension(:), intent(inout) :: convertTo
      integer i
      do i=lbound(convertFrom,1),ubound(convertFrom,1)
         convertTo(i)%v=convertFrom(i)
      end do
      end subroutine
      
      end module

