

      subroutine template()
      use OAD_cp
      use OAD_tape
      use OAD_rev

!$TEMPLATE_PRAGMA_DECLARATIONS

      integer :: cp_loop_variable_1,cp_loop_variable_2,
     +     cp_loop_variable_3,cp_loop_variable_4

      type(modeType) :: our_orig_mode

      integer iaddr
      external iaddr

      INTEGER numItersHelper
      INTEGER myThidHelper

      Real*8  cg2d_b_p(1-OLx:sNx+OLx,1-OLy:sNy+OLy,nSx,nSy)
      Real*8  cg2d_x_p(1-OLx:sNx+OLx,1-OLy:sNy+OLy,nSx,nSy)

      if (our_rev_mode%plain) then
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         cg2d_b_p=cg2d_b%v
         cg2d_x_p=cg2d_x%v
	call cg2d (
     I                cg2d_b_p,
     U                cg2d_x_p,
     O                firstResidual,
     O                lastResidual,
     U                numIters,
     I                myThid) 
        cg2d_x%v=cg2d_x_p        
c reset the mode
         our_rev_mode=our_orig_mode
      end if
      if (our_rev_mode%tape) then

         numItersHelper=numiters
         mythidHelper=mythid
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
         cg2d_b_p=cg2d_b%v
         cg2d_x_p=cg2d_x%v
	call cg2d (
     I                cg2d_b_p,
     U                cg2d_x_p,
     O                firstResidual,
     O                lastResidual,
     U                numIters,
     I                myThid) 
        cg2d_x%v=cg2d_x_p        
c reset the mode
         our_rev_mode=our_orig_mode
c manually push two integers to the tape:        
         if(oad_it_sz.lt. oad_it_ptr) call oad_it_grow()
         oad_it(oad_it_ptr)=numItersHelper; oad_it_ptr=oad_it_ptr+1
         if(oad_it_sz.lt. oad_it_ptr) call oad_it_grow()
         oad_it(oad_it_ptr)=mythidHelper; oad_it_ptr=oad_it_ptr+1
      end if
      if (our_rev_mode%adjoint) then
c manually pop two integers from the tape:        
      oad_it_ptr=oad_it_ptr-1
      mythid=oad_it(oad_it_ptr)
      oad_it_ptr=oad_it_ptr-1
      numiters=oad_it(oad_it_ptr)
c selfadjoint:
c the original is called with 
c  cg2d(b,x,...)
c in the adjoint context if we 
c use the same code base 
c we call with 
c  cg2d(x_bar,bh,...
c where afterwards
c b_bar+=bh  and x_bar=0
c the adjoint second formal argument cg2d_x should be
c the values of the first argument:  
      cg2d_b_p=cg2d_x%d
c the first formal argument cg2d_b should hold 
c the increment, i.e. we nullify the second formal 
c argument (cg2d_x) value:
      cg2d_x_p=0.0
c set up for plain execution
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
	call cg2d (
     I                cg2d_b_p,
     U                cg2d_x_p,
     O                firstResidual,
     O                lastResidual,
     U                numIters,
     I                myThid) 
c reset the mode
         our_rev_mode=our_orig_mode
c now the adjoint result is the increment 
c contained in the second formal argument
         cg2d_b%d= cg2d_b%d+cg2d_x_p
         cg2d_x%d=0.0
      end if 

      end subroutine template
