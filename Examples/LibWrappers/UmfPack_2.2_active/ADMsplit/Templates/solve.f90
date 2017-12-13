subroutine template()
  use OAD_tape
  use OAD_rev
!$TEMPLATE_PRAGMA_DECLARATIONS
  double precision , dimension(size(x)) :: passiveX, passiveB
  double precision , dimension(size(x),size(x)) :: passiveA
  integer :: i,j
  if (our_rev_mode%plain) then
     passiveB=b%v
     passiveA=A%v
     call solve(n,passiveA,passiveX,passiveB,trans)
     x%v=passiveX
  end if
  if (our_rev_mode%tape) then
     passiveB=b%v
     passiveA=A%v
     call solve(n,passiveA,passiveX,passiveB,trans)
     x%v=passiveX
     ! save x (do by hand what the inliner does)
     double_tape(double_tape_pointer:double_tape_pointer+size(passiveX)-1)=passiveX(:)
     double_tape_pointer=double_tape_pointer+size(passiveX)
     ! save A (do by hand what the inliner does)
     double_tape(double_tape_pointer:double_tape_pointer+ &
          (size(passiveA,1)*size(passiveA,2))-1)=reshape(passiveA,(/size(passiveA,1)*size(passiveA,2)/))
     double_tape_pointer=double_tape_pointer+(size(passiveA,1)*size(passiveA,2))
  end if
  if (our_rev_mode%adjoint) then
     ! for b: 
     passiveX=x%d
     ! restore A: (do by hand what the inliner does)
     double_tape_pointer=double_tape_pointer-(size(passiveA,1)*size(passiveA,2))
     passiveA(:,:)=reshape(double_tape(double_tape_pointer:),shape(passiveA))
     call solve(n,passiveA,passiveB,passiveX, .not. trans)  ! <---- flipping the transpose flag 
     b%d=b%d+passiveB
     ! for the matrix: decrement by outer product (\bar{b}, x^T)
     ! restore x (do by hand what the inliner does)
     double_tape_pointer=double_tape_pointer-size(passiveX)
     passiveX(:)=double_tape(double_tape_pointer:double_tape_pointer+size(passiveX)-1)
     do j=1,n
        do i=1,n
           A(i,j)%d=A(i,j)%d-passiveB(i)*passiveX(j)
        end do
     end do
     ! reset
     x%d=0.0
  end if
end subroutine template
