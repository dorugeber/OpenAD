! Shallow Water program
! Benchmark weather prediction program for comparing the
! preformance of current supercomputers. the model is
! based of the paper - the dynamics of finite-difference
! models of the shallow-water equations, by Robert Sadourny
! J. Atm. Sciences, vol 32, no 4, April 1975.
!
! Differences from APR's version:
! . Fortran 90-ized:
!	modules, internal subroutines, removing commons 
! . io_temp is mapped
! . some move-serial statments rearranged for message aggregation
!
! Notice that 
!     total computer time == compute + print diagonal and timing summary +
!                            (timing + sync) overhead in the subroutines
!	
!     total cpu time for job = sum of partial timings in each subroutine *
!                              number of iterations

module timer

  integer :: starting_count
  logical :: system_clock_not_yet_called = .true.

contains

  subroutine print_time
    character*8    :: date    ! ccyymmdd
    character*10   :: time    ! hhmmss.sss
    call date_and_time(date,time)
    print *, date(5:6)//'/'//date(7:8)//'/'//date(3:4)//   &
       '  '//time(1:2)//':'//time(3:4)//':'//time(5:10)
    return
  end subroutine print_time

  real function cputime()

	time = secnds(0.0) ! use a local variable to get around bug
	cputime = time
	return
  end function cputime

end module timer


module cons
  real    :: dt = 20.
  real    :: tdt,tdts8,tdtsdx,tdtsdy,fsdx,fsdy,tbeg
  real    :: dx = .25e5
  real    :: dy = .25e5
  real    :: a = 1.e6
  real    :: alpha = .001
  real    :: el
  real    :: pi
  real    :: tpi
  real    :: di
  real    :: dj
  real    :: pcf
  integer :: itmax = 50
  integer :: mprint = 50
  integer :: m
  integer :: n
  integer :: mp1
  integer :: np1
end module cons


program shallow
  ! benchmark weather prediction program for comparing the
  ! preformance of current supercomputers. the model is
  ! based of the paper - the dynamics of finite-difference
  ! models of the shallow-water equations, by Robert Sadourny
  ! J. Atm. Sciences, vol 32, no 4, April 1975.

  use timer
  use cons

  integer, parameter :: n1=513, n2=513
  !hpf$ template, dimension(n1, n2), distribute(*,block) :: cols
  real, dimension(n1,n2) ::     &
        u, v, p, unew, vnew, pnew, uold, vold, pold, cu, cv, z, h, psi
  !hpf$ align with cols ::     &
  !hpf$ u, v, p, unew, vnew, pnew, uold, vold, pold, cu, cv, z, h, psi

  real mfs100,mfs200,mfs300

  real io_temp(n2) ! temporary array to hold shrunk arrays for IO
  !hpf$ distribute io_temp(block)	

  open(unit=16,file='shallow.out')

  call initial  ! initialize constants and arrays

  write(16,390) n,m,dx,dy,dt,alpha,itmax     ! print initial values
  390 format('1number of points in the x direction', i8/   &
             ' number of points in the y direction', i8/   &
             ' grid spacing in the x direction    ', f8.0/ &
             ' grid spacing in the y direction    ', f8.0/ &
             ' time step                          ', f8.0/ &
             ' time filter parameter              ', f8.3/ &
             ' number of iterations               ', i8)
  mnmin = min0(m,n)

  ! move shrunk arrays to temporary for IO
  forall (i=1:mnmin)  io_temp(i)=pold(i,i)
  write(16,391) (io_temp(i),i=1,mnmin)
    391 format(/' initial diagonal elements of p ', //(8e15.7))

  forall (i=1:mnmin)  io_temp(i)=uold(i,i)
  write(16,392) (io_temp(i),i=1,mnmin)
    392 format(/' initial diagonal elements of u ', //(8e15.7))

  forall (i=1:mnmin)  io_temp(i)=vold(i,i)
  write(16,393) (io_temp(i),i=1,mnmin)
    393 format(/' initial diagonal elements of v ', //(8e15.7))

  ! determine overhead of timing calls
  tstart = cputime()
  t00 = cputime() - tstart
  call print_time     ! print date and time
  tstart = cputime()  ! initial call to cpu clock
  time = 0.
  ncycle = 0

  do
    ncycle = ncycle + 1

    call calc1    ! compute capital u, capital v, z and h

    call calc2    ! compute new values u, v and p
    
    time = time + dt
    if(mod(ncycle,mprint) == 0) then
      ptime = time/3600.
      write(16,350) ncycle, ptime
        350 format(//' cycle number',i5,' model time in  hours', f6.2)
      forall (i=1:mnmin)  io_temp(i)=pnew(i,i)
      write(16,355) (io_temp(i),i=1,mnmin)
        355 format(/' diagonal elements of p ', //(8e15.7))
      forall (i=1:mnmin)  io_temp(i)=unew(i,i)
      write(16,360) (io_temp(i),i=1,mnmin)
        360 format(/' diagonal elements of u ', //(8e15.7))
      forall (i=1:mnmin)  io_temp(i)=vnew(i,i)
      write(16,360) (io_temp(i),i=1,mnmin)
        365 format(/' diagonal elements of v ', //(8e15.7))

      ! compute mflops rates for the three major loops
      mfs100 = 0
      mfs200 = 0
      mfs300 = 0
      t100 = t100 - t00
      t200 = t200 - t00
      t300 = t300 - t00
      if(t100.gt.0 ) mfs100 = 24.*m*n*1.e-6/t100
      if(t200.gt.0 ) mfs200 = 26.*m*n*1.e-6/t200
      if(t300.gt.0 ) mfs300 = 15.*m*n*1.e-6/t300

      tend = cputime()
      ctime = tend - tstart - t00
      tcyc = ctime/float(ncycle)
      write(16,375) ncycle,ctime,tcyc,t100,mfs100,t200,mfs200,t300,mfs300
        375 format(' cycle number',i5,' total computer time', e15.6,  &
           ' time per cycle', e15.6 /                                 &
           ' time and megaflops for loop 100 ', e15.6,f8.3/           &
           ' time and megaflops for loop 200 ', e15.6,f8.3/           &
           ' time and megaflops for loop 300 ', e15.6,f8.3/ )
    end if

    ! test for end of run
    if (ncycle >= itmax) then
      tend = cputime()
      tottim = (t100+t200+t300)*ITMAX
      call print_time                    ! time stamp the end of run
      xflops = n*m*itmax*65*1.e-6/tottim ! compute mflops for the run
      write(6,9030) ctime,tottim,xflops
        9030  format(///'  total computer time', e15.6,/         &
	                '  total cpu time for job = ', e15.5,/   &
                        '  overall mflops rate    = ', e15.5)
      write(6,9010)
        9010  format(1h1)
          !*** write values of final iteration to unit 20 
      open(unit=20, file='shal.out')
	write(20,350) ncycle,ptime
	write(20,355) (pnew(i,i),i=1,mnmin)
	write(20,360) (unew(i,i),i=1,mnmin)
	write(20,365) (vnew(i,i),i=1,mnmin)
      stop
    endif

    ! time smoothing and update for next cycle
    if(ncycle <= 1) then
      call calc3z
    else
      call calc3
    endif
  end do

contains

  subroutine initial    ! initialize constants and arrays
    ! note below that two delta t (tdt) is set to dt on the first
    ! cycle after which it is reset to dt+dt.
    tdt = dt
    m = n1 - 1
    n = n2 - 1
    mp1 = m+1
    np1 = n+1
    el = float(n)*dx
    pi = 4.*atan(1.)
    tpi = pi+pi
    di = tpi/float(m)
    dj = tpi/float(n)
    pcf = pi*pi*a*a/(el*el)

    ! initial values of the stream function and p
    forall (j=1:np1, i=1:mp1)
      psi(i,j) = a*sin((float(i)-.5)*di)*sin((float(j)-.5)*dj)
      p(i,j) = pcf*(cos(2.*float(i-1)*di)   &
                   +cos(2.*float(j-1)*dj))+50000.
    end forall

    ! initialize velocities
    u(2:1 + m,:n) = -(psi(2:1 + m,2:1 + n) - psi(2:1 + m,:n)) / dy
    v(:m,2:1 + n) = (psi(2:1 + m,2:1 + n) - psi(:m,2:1 + n)) / dx

    ! periodic continuation
    u(1,:n) = u(m + 1,:n)
    v(m + 1,2:1 + n) = v(1,2:1 + n)
    u(2:1 + m,n + 1) = u(2:1 + m,1)           ! move-serial
    v(:m,1) = v(:m,n + 1)                     ! move-serial
    u(1,n + 1) = u(m + 1,1)
    v(m + 1,1) = v(1,n + 1)
    uold(:mp1,:np1) = u(:mp1,:np1)
    vold(:mp1,:np1) = v(:mp1,:np1)
    pold(:mp1,:np1) = p(:mp1,:np1)
! end of initialization
    return
  end subroutine initial


subroutine calc1  ! compute capital  u, capital v, z and h

      fsdx = 4. / dx
      fsdy = 4. / dy
      tbeg = cputime()
!CMIC$ DO GLOBAL
      cu(2:1 + m,:n) = .5 * (p(2:1 + m,:n) + p(:m,:n)) * u(2:1 + m,:n)
!NN p+
      cv(:m,2:1 + n) = .5 * (p(:m,2:1 + n) + p(:m,:n)) * v(:m,2:1 + n)
!NN u+, p+
      z(2:1 + m,2:1 + n) = (fsdx * (v(2:1 + m,2:1 + n) - v(:m,2:1 + n))  &
          - fsdy * (u(2:1 + m,2:1 + n) - u(2:1 + m,:n))) / (p(:m,:n)     &
          + p(2:1 + m,:n) + p(2:1 + m,2:1 + n) + p(:m,2:1 + n))
!NN v-
      h(:m,:n) = p(:m,:n) + .25 * (u(2:1 + m,:n) * u(2:1 + m,:n) +       &
          u(:m,:n) * u(:m,:n) + v(:m,2:1 + n) * v(:m,2:1 + n) + v(:m,:n) &
          *v(:m,:n))
      tend = cputime()
      t100 = tend - tbeg
!C
!C     PERIODIC CONTINUATION
!C
      cu(1,:n) = cu(m + 1,:n)
      cv(m + 1,2:1 + n) = cv(1,2:1 + n)
      z(1,2:1 + n) = z(m + 1,2:1 + n)
      h(m + 1,:n) = h(1,:n)
!CAPR$ DO PAR on H<:,1>
      cu(2:1 + m,n + 1) = cu(2:1 + m,1)  ! move-serial
      h(:m,n + 1) = h(:m,1)              ! move-serial
      cv(:m,1) = cv(:m,n + 1)            ! move-serial
      z(2:1 + m,1) = z(2:1 + m,n + 1)    ! move-serial
      cu(1,n + 1) = cu(m + 1,n+1)     ! modified  
      cv(m + 1,1) = cv(1,1)           ! modified  
      z(1,1) = z(m + 1,1)             ! modified  
      h(m + 1,n + 1) = h(1,n+1)       ! modified  
!C        END OF LOOP 100 CALCULATIONS
      return
end subroutine calc1

subroutine calc2   ! compute new values of u,v,p

      tdts8 = tdt / 8.
      tdtsdx = tdt / dx
      tdtsdy = tdt / dy
      tbeg = cputime()
!CMIC$ DO GLOBAL
!NN z-, cv-
      unew(2:1 + m,:n) = uold(2:1 + m,:n) + tdts8 * (z(2:1 + m,2:1 + n) &
          + z(2:1 + m,:n)) * (cv(2:1 + m,2:1 + n) + cv(:m,2:1 + n)      &
          + cv(:m,:n) + cv(2:1 + m,:n)) - tdtsdx * (h(2:1 + m,:n)       &
          - h(:m,:n))
!NN cu+, h+
      vnew(:m,2:1 + n) = vold(:m,2:1 + n) - tdts8 * (z(2:1 + m,2:1 + n) &
          + z(:m,2:1 + n)) * (cu(2:1 + m,2:1 + n) + cu(:m,2:1 + n)      &
          + cu(:m,:n) + cu(2:1 + m,:n)) - tdtsdy * (h(:m,2:1 + n)       &
          - h(:m,:n))
!NN cv-
      pnew(:m,:n) = pold(:m,:n) - tdtsdx * (cu(2:1 + m,:n) - cu(:m,:n)) &
          - tdtsdy * (cv(:m,2:1 + n) - cv(:m,:n))
      tend = cputime()
      t200 = tend - tbeg
!C
!C     PERIODIC CONTINUATION
!C
      unew(1,:n) = unew(m + 1,:n)
      vnew(m + 1,2:1 + n) = vnew(1,2:1 + n)
      pnew(m + 1,:n) = pnew(1,:n)
!CAPR$ DO PAR on PNEW<:,1>
      unew(2:1 + m,n + 1) = unew(2:1 + m,1)   ! move-serial
      pnew(:m,n + 1) = pnew(:m,1)             ! move-serial
      vnew(:m,1) = vnew(:m,n + 1)             ! move-serial
      unew(1,n + 1) = unew(m + 1,n+1)     ! modified
      vnew(m + 1,1) = vnew(1,1)           ! modified
      pnew(m + 1,n + 1) = pnew(1,n+1)     ! modified
!C
      return
end subroutine calc2

subroutine calc3z  ! time smoother for first iteration

      tdt = tdt + tdt
      uold(:mp1,:np1) = u(:mp1,:np1)
      u(:mp1,:np1) = unew(:mp1,:np1)
      vold(:mp1,:np1) = v(:mp1,:np1)
      v(:mp1,:np1) = vnew(:mp1,:np1)
      pold(:mp1,:np1) = p(:mp1,:np1)
      p(:mp1,:np1) = pnew(:mp1,:np1)
      return
end subroutine calc3z


subroutine calc3  ! time smoother

  ! time smoothing and update for next cycle
      tbeg = cputime()
!CMIC$ DO GLOBAL
      uold(:m,:n) = u(:m,:n) + alpha * (unew(:m,:n) - 2. * u(:m,:n)  &
         + uold(:m,:n))
      u(:m,:n) = unew(:m,:n)
      vold(:m,:n) = v(:m,:n) + alpha * (vnew(:m,:n) - 2. * v(:m,:n)  &
         + vold(:m,:n))
      v(:m,:n) = vnew(:m,:n)
      pold(:m,:n) = p(:m,:n) + alpha * (pnew(:m,:n) - 2. * p(:m,:n)  &
         + pold(:m,:n))
      p(:m,:n) = pnew(:m,:n)
      tend = cputime()
      t300 = tend - tbeg
!C
!C     PERIODIC CONTINUATION
!C
      uold(m + 1,:n) = uold(1,:n)
      vold(m + 1,:n) = vold(1,:n)
      pold(m + 1,:n) = pold(1,:n)
      u(m + 1,:n) = u(1,:n)
      v(m + 1,:n) = v(1,:n)
      p(m + 1,:n) = p(1,:n)
!CAPR$ DO PAR on POLD<:,1>
      uold(:m,n + 1) = uold(:m,1)      ! move-serial
      vold(:m,n + 1) = vold(:m,1)      ! move-serial
      pold(:m,n + 1) = pold(:m,1)      ! move-serial
      u(:m,n + 1) = u(:m,1)            ! move-serial
      v(:m,n + 1) = v(:m,1)            ! move-serial
      p(:m,n + 1) = p(:m,1)            ! move-serial
      uold(m + 1,n + 1) = uold(1,n+1)    ! modified
      vold(m + 1,n + 1) = vold(1,n+1)    ! modified
      pold(m + 1,n + 1) = pold(1,n+1)    ! modified
      u(m + 1,n + 1) = u(1,n+1)          ! modified
      v(m + 1,n + 1) = v(1,n+1)          ! modified
      p(m + 1,n + 1) = p(1,n+1)          ! modified
!C
      return
end subroutine calc3

end program shallow

