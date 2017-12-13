c$openad XXX Template ad_template.f
        subroutine head(x,y)
          implicit none
          double precision,dimension(1), target :: x
          double precision,dimension(1) :: y
          double precision,pointer:: a
c$openad INDEPENDENT(x)
         a=>x(1)
         a=a*2
         nullify(a)
         if (.not. associated(a)) then 
           allocate(a)
         end if
         a=x(1)
         a=a*2
         y(1)=a
c$openad DEPENDENT(y)
        end subroutine head
