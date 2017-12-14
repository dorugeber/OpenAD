      subroutine template()
      use OAD_tape
      use OAD_rev
      use OAD_cp

!$TEMPLATE_PRAGMA_DECLARATIONS

      integer :: cp_loop_variable_1,cp_loop_variable_2,
     +     cp_loop_variable_3,cp_loop_variable_4

      type(modeType) :: our_orig_mode

      integer iaddr
      external iaddr

      character*(80):: indentation='                                        
     +                                         '
      our_indent=our_indent+1
      write(*,'(A,A,L,L,L,L,L,L,L)') 
     +indentation(1:our_indent), "enter __SRNAME__:",
     +our_rev_mode%arg_store,
     +our_rev_mode%arg_restore,
     +our_rev_mode%res_store,
     +our_rev_mode%res_restore,
     +our_rev_mode%plain,
     +our_rev_mode%tape,
     +our_rev_mode%adjoint

c$$$          write(*,'(A,A,A,I16,A,I6,A,I6,A,I6,A,I8,A,I8)')
c$$$     +indentation(1:our_indent), "enter __SRNAME__:",
c$$$     +" AF:",theArgFStackoffset, 
c$$$     +" AI:",theArgIStackoffset, 
c$$$     +" RF:",theResFStackoffset, 
c$$$     +" RI:",theResIStackoffset, 
c$$$     +" DT:",double_tape_pointer, 
c$$$     +" IT:",integer_tape_pointer
c$$$
      if (our_rev_mode%arg_store) then 
         write(*,'(A,A)') 
     +indentation(1:our_indent), " __SRNAME__: entering arg store"
!$PLACEHOLDER_PRAGMA$ id=4
      end if 
      if (our_rev_mode%arg_restore) then
         write(*,'(A,A)') 
     +indentation(1:our_indent), " __SRNAME__: entering arg restore"
!$PLACEHOLDER_PRAGMA$ id=6
      end if
      if (our_rev_mode%plain) then
         write(*,'(A,A)') 
     +indentation(1:our_indent), " __SRNAME__: entering plain"
         our_orig_mode=our_rev_mode
         our_rev_mode%arg_store=.FALSE.
!$PLACEHOLDER_PRAGMA$ id=1
         our_rev_mode=our_orig_mode
      end if 
      if (our_rev_mode%tape) then
         write(*,'(A,A)') 
     +indentation(1:our_indent), " __SRNAME__: entering tape"
         our_rev_mode%arg_store=.TRUE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%res_store=.FALSE.
         our_rev_mode%res_restore=.FALSE.
         our_rev_mode%plain=.TRUE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.FALSE.
	 call timeratio()
!$PLACEHOLDER_PRAGMA$ id=2
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.FALSE.
         our_rev_mode%res_store=.FALSE.
         our_rev_mode%res_restore=.FALSE.
         our_rev_mode%plain=.FALSE.
         our_rev_mode%tape=.FALSE.
         our_rev_mode%adjoint=.TRUE.
      end if 
      if (our_rev_mode%adjoint) then
         write(*,'(A,A)') 
     +indentation(1:our_indent), " __SRNAME__: entering adjoint"
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.TRUE.
         our_rev_mode%res_store=.FALSE.
         our_rev_mode%res_restore=.FALSE.
         our_rev_mode%plain=.FALSE.
         our_rev_mode%tape=.TRUE.
         our_rev_mode%adjoint=.FALSE.
	 call timeratio()
!$PLACEHOLDER_PRAGMA$ id=3
         call timeratio()
         our_rev_mode%arg_store=.FALSE.
         our_rev_mode%arg_restore=.TRUE.
         our_rev_mode%res_store=.FALSE.
         our_rev_mode%res_restore=.FALSE.
         our_rev_mode%plain=.FALSE.
         our_rev_mode%tape=.TRUE.
         our_rev_mode%adjoint=.FALSE.
      end if 
c$$$          write(*,'(A,A,A,I16,A,I6,A,I6,A,I6,A,I8,A,I8)')
c$$$     +indentation(1:our_indent), "leave __SRNAME__:",
c$$$     +" AF:",theArgFStackoffset, 
c$$$     +" AI:",theArgIStackoffset, 
c$$$     +" RF:",theResFStackoffset, 
c$$$     +" RI:",theResIStackoffset, 
c$$$     +" DT:",double_tape_pointer, 
c$$$     +" IT:",integer_tape_pointer
      write(*,'(A,A,L,L,L,L,L,L,L)') 
     +indentation(1:our_indent), "leave __SRNAME__:",
     +our_rev_mode%arg_store,
     +our_rev_mode%arg_restore,
     +our_rev_mode%res_store,
     +our_rev_mode%res_restore,
     +our_rev_mode%plain,
     +our_rev_mode%tape,
     +our_rev_mode%adjoint
      our_indent=our_indent-1
      end subroutine template
