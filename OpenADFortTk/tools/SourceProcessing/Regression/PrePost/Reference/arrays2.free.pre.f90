module OAD_intrinsics
end module
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! A program with F77 and F90 style array operations
!
!
! ! Multiply two arrays element by element and assign the result to the
! ! third array.
! 
! ! Fortran 77 Solution
! REAL a(5, 5), b(5, 5), c(5, 5)
! DO 20 i = 1, 5
! DO 10 j = 1, 5
! c(j, i) = a(j, i) * b(j, i)
! 10 CONTINUE
! 20 CONTINUE
! 
! ! Fortran 90 Solution
! REAL, DIMENSION (5, 5) :: a, b, c
! c = a * b
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! Assign all the elements of array a to be zero, then perform a computation.
! assignment a(i) = a(i)/3.1 + b(i)*SQRT(c(i)) for all i.
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!
! Fortran 77 Solution
subroutine compute1()
     use OAD_intrinsics
     REAL x
     REAL a(20), b(20), c(20)
     INTEGER ti(5)
     REAL tr(5)

     x = 5

     tr(iabs((1))) = x
     tr(2) = 9
     tr(3) = 68

     DO 10 i=1,20
       ti(1) = i
       a(ti(1)) = tr(1)
       b(ti(1)) = tr(2)
       c(ti(1)) = tr(3)
 10        CONTINUE
        

      DO 20 i=1,20
        a(i) = (a(i)/3.1)+(b(i)*SQRT(c(i)))
        WRITE(*,*) 'a(', i, ')=', a(i)
 20         CONTINUE
end subroutine

! Fortran 90 Solution
subroutine compute2()
  use OAD_intrinsics
  implicit none
  real :: x
  real, dimension(20) :: a, b, c
  x = 5

  a = x
  b = 9
  c = 68
  a = (a/3.1)+(b*SQRT(c))
  write(*,*) 'a =', a
end subroutine


program f77_f90_arrays
  use OAD_intrinsics
  double precision :: x, y
  x = 2
  y = 8

  call compute1()
  call compute2()

end program
