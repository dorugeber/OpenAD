      subroutine foo(x)
        real,dimension(10) :: x
        
        x = x ** 2
      end subroutine foo

      integer function bar(i,j,k)
        integer :: i,j,k
        integer itmp1,itmp2
        
        itmp1 = 8 * j
        itmp2 = 4 + i
        
        bar = itmp1 + itmp2 * k
        
      end function bar
      
      module bar1
        real(kind = 4),dimension(4,5) :: r,rr(3),vvv
        integer*4   ival1,ival2
      end module bar1
