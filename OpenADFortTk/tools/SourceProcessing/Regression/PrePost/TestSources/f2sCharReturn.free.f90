module m 
contains
  character*16 function foo (l) 
    logical :: l
    if (l) then
       foo = '      Yes       '
    else
       foo = '      No        '
    end if
  end function foo
end

program p 
  use m 
  logical :: l 
  l=.true.
  write (*,*) foo(l)
end  
