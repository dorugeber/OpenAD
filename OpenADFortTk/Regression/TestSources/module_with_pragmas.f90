module active_module
  implicit none

  interface aMethod
     module procedure aMethodImpl
  end interface

  interface aSecondMethod
     module procedure aSecondMethodImpl
  end interface

  interface aThirdMethod
     module procedure aThirdMethodImpl
  end interface

  contains 

! this is to look at the location of the pragma
! only meaningful in conjunction with the postprocessor

!$openad xxx afirstpragma
    subroutine aMethodImpl(i)
      integer i
      print*, 'aMethodImpl', i
    end subroutine 
    
!$openad xxx asecondpragma
    subroutine aSecondMethodImpl(i)
      integer i
      print*, 'aSecondMethodImpl', i
    end subroutine 

    subroutine aThirdMethodImpl(i)
!$openad xxx asthirdpragma
      integer i
      print*, 'aThirdMethodImpl', i
    end subroutine 
    
end module active_module

program test 
  use active_module
  call aMethod(1)
  call aSecondMethod(2)
  call aThirdMethod(3)
end program
