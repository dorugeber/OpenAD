       program main

       integer i
       real*8 a(100), b(100)
       do i=1,100
         a(i)=b(i)*jak(i)
       enddo
       print *,a(i) 
       end

       function jak (a)
       integer a

       if (a < 15) then
         jak=a
       else
         jak=2
       endif

       return
       end


