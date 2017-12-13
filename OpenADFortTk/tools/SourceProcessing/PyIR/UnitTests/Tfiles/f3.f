c
c
c  actual comment lines


      subroutine foo(x)
c  1st statement!
      x = x + 5 + 2 * x
c
c not embedded continuation lines
c
      x = 5.0
      x =  13.2
      open(15,file='Thisone')
 10   format(10x,3f6.4)

      do i = 1,12
          yy(i) = 2*i+7
      enddo

c
c the 'end' of the line
      end
