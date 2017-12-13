        subroutine template()
          use OAD_tape
          use OAD_rev
          use lu , trueSolve => solve
!$TEMPLATE_PRAGMA_DECLARATIONS

          integer iaddr
          external iaddr
          
          double precision , dimension(size(x)) :: passiveX, passiveB

          if (our_rev_mode%plain) then
            passiveB=b%v
            call trueSolve(A,passiveX,passiveB)
            x%v=passiveX
          end if
          if (our_rev_mode%tape) then
            passiveB=b%v
            call trueSolve(A,passiveX,passiveB)
            x%v=passiveX
          end if 
          if (our_rev_mode%adjoint) then
            passiveX=x%d
            call trueSolve(A,passiveB,passiveX)
            b%d=b%d+passiveB
          end if 
        end subroutine template
