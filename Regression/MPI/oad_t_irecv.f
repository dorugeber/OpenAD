        subroutine template()
          use OAD_tape
          use OAD_rev

!$TEMPLATE_PRAGMA_DECLARATIONS

          integer lcount
          integer lsrc
          integer iaddr
          external iaddr

         if (our_rev_mode%plain) then
! original function
            call mpi_irecv( 
     +      buf,  
     +      count*2, 
     +      datatype, 
     +      src, 
     +      tag, 
     +      comm, 
     +      request, 
     +      ierror)
          end if
          if (our_rev_mode%tape) then
! taping
          integer_tape(integer_tape_pointer) = count
          integer_tape_pointer = integer_tape_pointer+1
          integer_tape(integer_tape_pointer) = src
          integer_tape_pointer = integer_tape_pointer+1
            call mpi_irecv( 
     +      buf,  
     +      count*2, 
     +      datatype, 
     +      src, 
     +      tag, 
     +      comm, 
     +      request, 
     +      ierror)
          end if 
          if (our_rev_mode%adjoint) then
! adjoint
          integer_tape_pointer = integer_tape_pointer-1
          lsrc = integer_tape(integer_tape_pointer)
          integer_tape_pointer = integer_tape_pointer-1
          lcount = integer_tape(integer_tape_pointer)
            call oadtisend( 
     +      buf,  
     +      lcount*2, 
     +      datatype, 
     +      lsrc, 
     +      tag, 
     +      comm, 
     +      request, 
     +      ierror)
          end if 

        end subroutine template
