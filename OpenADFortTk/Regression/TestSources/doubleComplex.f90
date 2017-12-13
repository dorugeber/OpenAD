program p 
    double complex :: dc
    double precision :: i1, p1, p2
    i1=1.0D0
    dc=cmplx(0.0D0, i1)
    p1=real(dc)
    p2=aimag(dc)
    print *,dc, p1, p2
end program
