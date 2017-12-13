subroutine ex(a,b)
real a,b,c,d,e
c=a*b
e=c
if (c>0) then
  d=a*c
  c=e*d
  c=c+b
  b=c
else  
  e=c
  d=a*c
  b=e
endif
end

program controlflow_if2
real a,b
a=2.0D0
b=3.2D0
call ex(a,b)
print *,a,b
end program
