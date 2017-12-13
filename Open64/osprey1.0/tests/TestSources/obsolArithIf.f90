program arithIf	
      integer i
      i=-3
      if (i) 2,3,4
4     print*, 'failed'
      go to 5
3     print*, 'failed'
      go to 5
2     print*, 'OK'
5     i=1
end program
