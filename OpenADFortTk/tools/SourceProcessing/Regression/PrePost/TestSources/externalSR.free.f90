subroutine foo ()
   integer, save :: fooi=1
   fooi=fooi+1
   print *, fooi
end subroutine


subroutine bar(f)
  external f
  call f()
end subroutine 

subroutine  head ()
  external foo
  call bar(foo)
end subroutine 

program p 
call head()
end program
