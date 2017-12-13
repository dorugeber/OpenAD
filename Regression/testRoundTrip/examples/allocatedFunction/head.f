c$openad XXX Template ad_template.f
        subroutine head(x,y)
          implicit none
          double precision,dimension(2) :: x
          double precision,dimension(1) :: y
          double precision,dimension(:),allocatable :: a
c$openad INDEPENDENT(x)
          allocate(a(2))
          a(1) = x(1)*2
          a(2) = x(2)*2
          if(allocated(a)) then
            y(1) = a(1)*a(2)
          else
            y(1) = 0
          endif
          deallocate(a)
c$openad DEPENDENT(y)
        end subroutine head
