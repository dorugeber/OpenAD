subroutine foo(i)
print *,i
end 

program p 
implicit integer (f)
integer i
fun(l)=i+l
i=3
call foo(fun(2))
end

