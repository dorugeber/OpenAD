subroutine foo(a,b) 

  character(*) :: a,b
  if (lge(a,b)) then 
    print *,.true.
  else 
    print *,.false.
  end if
  if (lgt(a,b)) then
    print *,.true.
  else
    print *,.false.
  end if
  if (lle(a,b)) then
    print *,.true.
  else
    print *,.false.
  end if
  if (llt(a,b)) then
    print *,.true.
  else
    print *,.false.
  end if 

end subroutine

program p 
  call foo("x ","x")
end program  
