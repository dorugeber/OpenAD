subroutine template()
  use OAD_tape
  use OAD_rev
!$TEMPLATE_PRAGMA_DECLARATIONS
  double precision , dimension(size(x)) :: passiveX, passiveB
  if (our_rev_mode%plain) then
     passiveB=b%v
     call solve(n,A,passiveX,passiveB,trans)
     x%v=passiveX
  end if
  if (our_rev_mode%tape) then
     passiveB=b%v
     call solve(n,A,passiveX,passiveB,trans)
     x%v=passiveX
  end if
  if (our_rev_mode%adjoint) then
     passiveX=x%d
     call solve(n,A,passiveB,passiveX, .not. trans)  ! <---- flipping the transpose flag 
     b%d=b%d+passiveB
     x%d=0.0
  end if
end subroutine template
