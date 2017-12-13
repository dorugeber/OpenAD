!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A program with type kinds (using parameters)
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

  !------------------------------------------------------------
  ! module consts
  !    save
  !    integer PI_C = PI
  !    ...
  ! end module consts
  !
  ! save
  !   integer ad_4 = ad_4_p
  !   integer ad_8 = ad_8_p
  !   integer ad_16 = ad_16_p
  !------------------------------------------------------------

subroutine compute(x, y)
  use ad_types
  implicit none
  
  real(ad_8), intent(inout) :: x, y
  x = x / y
  y = ( y * .789 ) - 2
end subroutine compute

program simple3
  use ad_types
  implicit none

  real(ad_8) :: x, y
  x = 2
  y = 8

  call compute(x, y)

end program

