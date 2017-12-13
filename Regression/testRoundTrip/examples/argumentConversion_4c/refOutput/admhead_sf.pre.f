!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      subroutine foo(x,y)
        use OAD_intrinsics
        double precision x
        double precision y
        y = x*x
      end subroutine

C$openad XXX Template ad_template.f
      subroutine head(x,y)
      use OAD_intrinsics
      double precision, dimension(2) :: x
      double precision, dimension(2,2) :: ax,apx
      double precision y,py
      integer i,j
C$openad INDEPENDENT(x)
C$openad xxx simple loop
      do i=1,2
         do j=1,2
            ax(i,j)=x(i)
            apx(i,j) = i*4.0
         end do
      end do
C$openad xxx simple loop
      do i=1,2
        do j=1,2
         call foo(ax(i,j),y)
         call foo(apx(i,j),py)
          end do
      end do
C$openad DEPENDENT(y)
      end subroutine

