c$openad XXX Template ad_template.f
        subroutine head(x,y) 
        double precision, dimension(2) :: x
        double precision, dimension(2) :: y
        double precision, dimension(size(x)) :: t
        double precision, dimension(size(y)) :: u
c$openad INDEPENDENT(x)
        t = sin(x)
        u = 5+x*t
        y = sqrt(u)
c$openad DEPENDENT(y)
        end subroutine

