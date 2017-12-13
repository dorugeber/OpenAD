        subroutine template()
          use lu , trueTest => test
!$TEMPLATE_PRAGMA_DECLARATIONS
          
            call trueTest(A,x,b)
        end subroutine template
