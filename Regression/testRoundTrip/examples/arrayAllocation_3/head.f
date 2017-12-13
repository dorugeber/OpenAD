c$openad XXX Template ad_template.f
        subroutine head(x,y) 
        double precision, dimension(2) :: x
        double precision, dimension(2) :: y
        double precision, dimension(size(x)) :: a,b,c
c$openad INDEPENDENT(x)
        a = tan(x)/sin(x)
        b = a*x+cos(8*x)
        b = b*(tan(x)*a)
        c = a**(4/a)-sqrt(x*a+cos(a)*tan(b))
        y = 5*c+a
c$openad DEPENDENT(y)
        end subroutine

