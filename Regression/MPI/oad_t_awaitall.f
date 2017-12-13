        subroutine template()
          use OAD_tape
          use OAD_rev

!$TEMPLATE_PRAGMA_DECLARATIONS

          integer iaddr
          external iaddr
          integer i
          integer tReq(count)

         if (our_rev_mode%plain) then
! original function
! do nothing
          end if
          if (our_rev_mode%tape) then
! taping
! do nothing
          end if 
          if (our_rev_mode%adjoint) then
                                ! adjoint
             tReq(1:count)=requests(1:count)
            call mpi_waitall( 
     +      count, 
     +      requests, 
     +      statuses, 
     +      ierror)
            do i=1,count
               call oadhandlerequest(tReq(i))
            end do
          end if 
        end subroutine template
