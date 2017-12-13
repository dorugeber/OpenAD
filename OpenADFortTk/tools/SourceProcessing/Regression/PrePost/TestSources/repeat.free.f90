subroutine foo(xxx)
  character :: xxx(3)
  print *,xxx
end subroutine

program p 
  character :: xxx(3),x
  x='x'
  call foo(repeat(x,3))
end program
  
