function foo(i) 
  real i
  real foo
  foo=i
end function 

program p 
interface 
function foo(i)
  real i
  real foo
end function
end interface
x=foo(3.0)
print *,x
end program
