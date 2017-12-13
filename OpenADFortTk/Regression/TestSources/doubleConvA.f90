program p 
    integer :: i(2)
    double precision :: d(2)
    data i/1,2/
    d=DBLE(i)
    print *,d
end program
