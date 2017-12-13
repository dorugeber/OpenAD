!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine head(x,fvec)
      use OAD_intrinsics
      double precision x(4), fvec(11)
C     **********
C
C     Subroutine daerfj
C
C     This subroutine computes the function and the Jacobian matrix
C     of the analysis of an enzyme reaction problem.
C
C     **********
      integer i
      double precision temp1, temp2
      double precision v(11), y(11)

C$openad INDEPENDENT(x)

      v(1)=4.0d0
      v(2)=2.0d0
      v(3)=1.0d0
      v(4)=5.0d-1
      v(5)=2.5d-1
      v(6)=1.67d-1
      v(7)=1.25d-1
      v(8)=1.0d-1
      v(9)=8.33d-2
      v(10)=7.14d-2
      v(11)=6.25d-2
      y(1)=1.957d-1
      y(2)=1.947d-1
      y(3)=1.735d-1
      y(4)=1.6d-1
      y(5)=8.44d-2
      y(6)=6.27d-2
      y(7)=4.56d-2
      y(8)=3.42d-2
      y(9)=3.23d-2
      y(10)=2.35d-2
      y(11)=2.46d-2

      do i = 1, 11
         temp1 = v(i)*(v(i)+x(2))
         temp2 = v(i)*(v(i)+x(3))+x(4)
         fvec(i) = y(i)-x(1)*temp1/temp2
      end do
C$openad DEPENDENT(fvec)
      end subroutine
