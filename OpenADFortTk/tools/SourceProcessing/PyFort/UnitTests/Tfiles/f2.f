c
c
c  actual comment lines


      subroutine foo(x)
c  1st statement!
      x = x +
c
c embedded continuation lines
c
     &5 + 2 * x
c
c more embedded lines
c
     & + si
     &n(x+2.0)
      x = 5.0
      x
     & = 
     & 13.2

c
c the 'end' of the line
      end
