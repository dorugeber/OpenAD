module bla
end module

subroutine head(x,y)
 integer, dimension(:,:) :: x
 integer, dimension(10) :: y
 y = reshape(x,shape(y))
end subroutine

program main
 interface 
  subroutine head(x,y)
   integer, dimension(:,:) :: x
   integer, dimension(10) :: y
  end subroutine
 end interface
 integer, dimension(2,5) :: x
 integer, dimension(10) :: y
 x(2,3)=6
 call head(x,y)
 if (y(6) .eq. 6) then 
    write(*,*) 'OK'
 else
    write(*,*) 'failed; yields', y(6), ' expected 6'
 end if 
end program
