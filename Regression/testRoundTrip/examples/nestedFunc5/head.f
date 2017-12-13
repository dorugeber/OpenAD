      module pcalc_I   
        implicit none
      contains
        subroutine pcalc ( x, y)
          integer, dimension(1) :: x, y
          integer :: p
          p  = 1 * y(1)
          x(1)  = f ( p )
          return  
        contains 
          integer function f (p) 
            integer , intent(in) :: p
            f = p*2
            return  
          end function f
        end subroutine pcalc
      end module pcalc_I   

      module pxcalc_I
        implicit none
      contains
        subroutine pxcalc ( x, y )
          integer, dimension(1) :: x, y
          integer :: p, q
          p  = 1 * y(1)
          q  = 2 * y(1)
          x(1)  = f ( p, q )
          return  
        contains 
          integer function f (p, q) 
            integer , intent(in) :: p,q
            f = p*q*2
            return  
          end function f
        end subroutine pxcalc
      end module pxcalc_I   
