module m 

 type t 

   real, pointer :: a(:,:) 

end type 

contains 

  subroutine foo(aT) 
    type(t) :: aT 

    allocate (aT%a(2,2))
    aT%a=0.0
    write (*,*)  aT%a(2,2)
  end subroutine 
end module 

program p 
  use m 
  type(t) :: aT
  call foo(aT)

end program
