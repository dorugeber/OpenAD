C$openad XXX Template ad_template.f
      subroutine head(x,y)
        double precision,intent(in),dimension(2),target :: x
        double precision,intent(out),dimension(2) :: y

        double precision,dimension(:),pointer :: p
   
C$openad INDEPENDENT(x)
        p => x
        y = x + p
C$openad DEPENDENT(y)
      end subroutine

