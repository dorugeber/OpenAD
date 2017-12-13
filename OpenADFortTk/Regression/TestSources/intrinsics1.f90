!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A program to test intrinsics on argument different types.
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


module ad_types

integer :: ad_4, ad_8, ad_16
parameter (ad_4 = kind(0.0))
parameter (ad_8 = kind(0.0d0))
parameter (ad_16 = selected_real_kind(p=30))

integer :: ad_i1, ad_i2, ad_i4,ad_i8
parameter (ad_i1 = selected_int_kind(r=2))
parameter (ad_i2 = selected_int_kind(r=3))
parameter (ad_i4 = selected_int_kind(r=8))
parameter (ad_i8 = selected_int_kind(r=16))

end module ad_types


program intrinsics
  use ad_types
  implicit none
  
  real(ad_4) :: x = 2.
  real(ad_8) :: yy = 3.
  real(ad_8) :: zzz = 4.  ! ad_16 causes problems with WHIRL

  character(len=3) :: a = 'aaa', b = 'bbb'

  !double precision :: a(20)
  !a = 5
  !a = cos(a)

  if ( a < b ) then
    zzz = 5.
  endif

  x   = sqrt(x)
  yy  = sqrt(yy)

  x   = sin(x)
  yy  = sin(yy)
  zzz = sin(zzz)
  
  x   = cos(x)
  yy  = cos(yy)
  zzz = cos(zzz)
  
  x   = exp(x)
  yy  = exp(yy)
  zzz = exp(zzz)
  
  x   = log(x)   ! ln
  yy  = log(yy)  ! ln
  zzz = log(zzz) ! ln
  
  x   = x**x     ! pow
  yy  = yy**yy   ! pow
  zzz = zzz**zzz ! pow
  
  write(*,*) x
  write(*,*) yy
  write(*,*) zzz
  
end program intrinsics
