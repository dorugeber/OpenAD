subroutine template()
  use OAD_tape
  use OAD_rev
  use OAD_cp
  use revolve
  !$TEMPLATE_PRAGMA_DECLARATIONS
  LOGICAL :: initialized=.FALSE.
  TYPE(rvAction),save :: theAction
  CHARACTER , DIMENSION(80) :: errorMsg
  integer, save :: jointCPCount
  integer, save :: currIter

  if (our_rev_mode%plain) then
     ! repeat loop contents from here, but this is just for illustration as we won't use it
     usrfold  = usrf
     thckold  = thck
     dthckdtm%v = 0.0
     dusrfdtm%v = 0.0
     bwat = 0.0
     bmlt%v = 0.0
     do t=1,nt
        call body(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon,thckold,usrfold,dthckdtm, &
       dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, &
       calving_flag,clim_flag,outfreq,t)
     enddo
     ! repeat loop contents to here
  end if
  if (our_rev_mode%tape) then
     ! repeat steps before the loop contents  here since we are in outer split mode
     usrfold  = usrf
     thckold  = thck
     dthckdtm%v = 0.0
     dusrfdtm%v = 0.0
     bwat = 0.0
     bmlt%v = 0.0
     ! repeated until here
     currIter=0
     jointCPcount=cp_fNumber()
     initialized=rvInit(nt,4,errorMsg,theAction)
     call rvVerbose(0)
     IF (.NOT.initialized) WRITE(*,'(A,A)') 'Error: ', errorMsg
     do while (theAction%actionFlag/=rvDone)
        theAction=rvNextAction()
        select case (theAction%actionFlag) 
        case (rvStore)
           call cp_write_open(theAction%cpNum+jointCPCount)
           our_rev_mode%arg_store=.TRUE.
           our_rev_mode%tape=.FALSE.
           call body(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon,thckold,usrfold,dthckdtm, &
                dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, &
                calving_flag,clim_flag,outfreq,currIter+1)
           our_rev_mode%arg_store=.FALSE.
           our_rev_mode%tape=.TRUE.
           call cp_close
        case (rvForward)
           call OAD_revPlain
           do currIter=currIter,theAction%iteration-1
              call body(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon,thckold,usrfold,dthckdtm, &
                   dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, &
                   calving_flag,clim_flag,outfreq,currIter+1)
           end do
           call OAD_revTape
        case (rvFirstUTurn)
           call body(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon,thckold,usrfold,dthckdtm, &
                dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, &
                calving_flag,clim_flag,outfreq,currIter+1)
           ! get out now ...
           exit                
        end select
     end do
  end if
  if (our_rev_mode%adjoint) then
     IF (.NOT.initialized) WRITE(*,'(A)') 'Error: not initialized'
     do while (theAction%actionFlag/=rvDone)
        select case (theAction%actionFlag) 
        case (rvFirstUTurn)
           !we taped already ... see above ... do the adjoint
           call body(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon,thckold,usrfold,dthckdtm, &
       dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag,calving_flag,clim_flag,outfreq,currIter+1)
        case (rvStore)
           call cp_write_open(theAction%cpNum+jointCPCount)
           our_rev_mode%arg_store=.TRUE.
           our_rev_mode%adjoint=.FALSE.
           call body(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon,thckold,usrfold,dthckdtm, &
                dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, &
                calving_flag,clim_flag,outfreq,currIter+1)
           our_rev_mode%arg_store=.FALSE.
           our_rev_mode%adjoint=.TRUE.
           call cp_close
        case (rvRestore)
           call cp_read_open(theAction%cpNum+jointCPCount)
           our_rev_mode%arg_restore=.TRUE.
           our_rev_mode%adjoint=.FALSE.
           call body(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon,thckold,usrfold,dthckdtm, &
                dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, &
                calving_flag,clim_flag,outfreq,currIter+1)
           our_rev_mode%arg_restore=.FALSE.
           our_rev_mode%adjoint=.TRUE.
           currIter=theAction%iteration
           call cp_close
        case (rvForward)
           call OAD_revPlain
           do currIter=currIter,theAction%iteration-1
              call body(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon,thckold,usrfold,dthckdtm, &
                   dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, &
                   calving_flag,clim_flag,outfreq,currIter+1)
           end do
           call OAD_revAdjoint
        case (rvUTurn)
           call OAD_revTape
           call body(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon,thckold,usrfold,dthckdtm, &
                dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, &
                calving_flag,clim_flag,outfreq,currIter+1)
           call OAD_revAdjoint
           call body(n,m,nz,dx,dy,dt,nt,sigma,depth,thck,topg,usrf,prcp,artm_pres,lat,lon,thckold,usrfold,dthckdtm, &
                dusrfdtm,diff,pddtab,bwat,bmlt,temp,ncdf_out,G,vol,ncdata,nmsb_ctrl,bslide_ctrl,enFac,slide_flag, &
                calving_flag,clim_flag,outfreq,currIter+1)
        end select
        theAction=rvNextAction()
     end do
  end if
end subroutine template

      
