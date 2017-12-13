      double precision function fltrn() result(z)

        integer :: k
        integer,parameter :: r1 = 12211, m1 = 2147483563
        integer,parameter :: a1 = 40014, q1 = 53668
        integer,parameter :: r2 = 3791, m2 = 2147483399
        integer,parameter :: a2 = 40692, q2 = 52774
        k      = s1/q1
        s1     = a1*(s1-k*q1) - k*r1
        if ( s1.lt.0 ) s1     = s1 + m1
        k      = s2/q2
        s2     = a2*(s2-k*q2) - k*r2
        if ( s2.lt.0 ) s2     = s2 + m2
        z      = s1 - s2
        if ( z.lt.1 )  z      = z + m1 - 1
        z      = z/m1
      end function fltrn
