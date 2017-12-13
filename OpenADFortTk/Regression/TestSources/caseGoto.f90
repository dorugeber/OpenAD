subroutine getLabel(j)
  integer , save :: i=5
  integer j
  i=i-1
  j=i
  return
end subroutine

program cg
  integer :: v=1,i
  1 call getLabel(i)
  print *,i
  select case (i)
  case (2)  
     goto 2
  case (3)
     v=v-1
     goto 1
  case (4)  
     v=v*2
     goto 1
  case default 
	stop "problem"
  end select
  2 print *,v
end program
