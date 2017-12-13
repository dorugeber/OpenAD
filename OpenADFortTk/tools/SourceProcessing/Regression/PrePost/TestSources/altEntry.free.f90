function foo1(x)
  double precision:: x,foo1,foo2
  foo1=x
  return 
entry foo2(x)
  foo2=2*x
end 

program p 
  double precision :: x, foo1, foo2
  external:: foo1,foo2
  x=2.0
  x=foo1(x)
  print *,x
  x=foo2(x)
  print *,x
end
