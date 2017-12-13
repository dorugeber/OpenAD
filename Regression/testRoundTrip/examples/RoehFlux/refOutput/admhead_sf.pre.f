!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      SUBROUTINE ad_roehf5 (nrm, priml, primr, gamma, gm1, gm1inv, nlefi
     +x, lefix, mcheps, flux)
      use OAD_intrinsics
C     !*****************************************************************
C     !  SUBROUTINE: AD_ROE_HARTEN_FLUX5_EXP                            
C     !      AUTHOR: Shaun Forth                                        
C     !        DATE: 050997                                             
C     !   COPYRIGHT: RMCS, Cranfield University 1997                    
C     !    ABSTRACT: For MUSCL interpolated cell face primitive values  
C     !              calculates explicit euler flux for 3-D inviscid gas
C     !              turbulence  model (i.e. 5 variables).              
C     !                                                                 
C     !    MODIFIED: Uwe Naumann, Feb 04                                 
C       
C     !                                                                 
C     !*****************************************************************
      IMPLICIT none
C     AUGUMENTS(in):                                                    
C     nrm    : cell face normal                                         
C     priml  : primitive variables left of cell face                    
C     primr  : primitive variables right of cell face

      double precision,intent(in):: nrm (3)
      double precision,intent(in):: priml (5), primr (5)
C     gamma  : ratio of specific heats                                  
C     gm1    : gamma - 1                                                
C     gm1inv : 1/gm1                                                    
C     nlefix : non-linear entropy fix parameter                         
C     lefix  : linear entropy fix parameter                             
C     mcheps : machine epsilon

      double precision,intent(in):: gamma, gm1, gm1inv, nlefix, lefix, m
     +cheps


C     AUGUMENTS(out):                                                   
C     flux  : inviscid flux

      double precision,intent(out):: flux (5)

C     PARAMETERS

      real:: one, half, zero
      PARAMETER (one = 1.0d0, half = 0.5d0, zero = 0.0d0)

C     LOCAL VARIABLES                                                   
C     independent of flow variables                                     
C     nsize,nsizei      : size of normal and reciprocal                 
C     nxhat,nyhat,nzhat : components of normalised normal

      double precision:: nsize, nsizei, nxhat, nyhat, nzhat

C     dependent on flow variables                                       
C     roel,roer  : Roe averaging weights                                
C     thetal,thetar :  |ul^2| |ur|^2                                    
C     hl,hr         : left and right specific enthalpies                
C     uave,vave,wave,have,cave : Roe average values                     
C     uhat,thtave              : uave.nhat, 0.5|uave^2|                 
C     utilde,vtilde,wtilde     : nhat x u                               
C     delta,delta2             : size of entropy fix                    
C     lamcm,lamcp,lamu         : raw eigenvalues/wavespeeds             
C     lam2                     : eigenvalue**2                          
C     alamcm,alamcp,alamu      : entropy fixed eigenvalues/wavespeeds   
C     el,rul,rvl,rwl           : left conservative variables            
C     er,rur,rvr,rwr           : right conservative variables           
C     de,dr,dru,drv,drw       :  change in conservative variables       
C     uddru,omega,nddru        : intermediate wave-strength vars.       
C     alp1,alp2,alp3,alp4,alp5 : wave-strengths                         
C     alp15p,alp15m            : alam1*alp1+/-alam5*alp5                
C     mu                       : used for dissipation vector            
C     dss1,dss2,dss3,dss4,dss5 : dissipation vector                     
C     uhatl                    : u.nhat left side of face

      double precision:: roel, roer
      double precision:: thetal, thetar
      double precision:: hl, hr
      double precision:: uave, vave, wave, have, cave
      double precision:: uhat, thtave
      double precision:: utilde, vtilde, wtilde
      double precision:: delta, delta2
      double precision:: lamcm, lamcp, lamu
      double precision:: lam2
      double precision:: alamcm, alamcp, alamu
      double precision:: el, rul, rvl, rwl
      double precision:: er, rur, rvr, rwr
      double precision:: de, dr, dru, drv, drw
      double precision:: uddru, omega, nddru
      double precision:: alp1, alp2, alp3, alp4, alp5
      double precision:: alp15p, alp15m
      double precision:: mu
      double precision:: dss1, dss2, dss3, dss4, dss5
      double precision:: uhatl

C$openad INDEPENDENT(nrm)
C$openad INDEPENDENT(priml)
C$openad INDEPENDENT(primr)
C$openad INDEPENDENT(gamma)
C$openad INDEPENDENT(gm1)
C$openad INDEPENDENT(gm1inv)
C$openad INDEPENDENT(nlefix)
C$openad INDEPENDENT(lefix)
C$openad INDEPENDENT(mcheps)
C
C     CODE                                                              
C                                                                       
C     set normal quantities                                             
C UN changed      nsize = sqrt (nrm (1) **2 + nrm (2) **2 + nrm (3) **2)

      nsize = sin (nrm (1) **2 + nrm (2) **2 + nrm (3) **2)
      IF (nsize.gt.mcheps) then
                nsizei = one/nsize
      ELSE
                nsizei = zero
      ENDIF
      nxhat = nrm(1)*nsizei
      nyhat = nrm(2)*nsizei
      nzhat = nrm(3)*nsizei

C     Roe weights                                                       
C UN changed      roel = one / (one+sqrt (primr (2) / priml (2) ) )

      roel = one/(one+sin(primr(2)/priml(2)))
      roer = one-roel

C     enthalpies

      thetal = half*(priml(3)**2+priml(4)**2+priml(5)**2)
      thetar = half*(primr(3)**2+primr(4)**2+primr(5)**2)
      hl = (gamma*gm1inv)*priml(1)/priml(2)+thetal
      hr = (gamma*gm1inv)*primr(1)/primr(2)+thetar

C     Roe average

      uave = roel*priml(3)+roer*primr(3)
      vave = roel*priml(4)+roer*primr(4)
      wave = roel*priml(5)+roer*primr(5)
      have = roel*hl+roer*hr
      thtave = half*(uave**2+vave**2+wave**2)
      cave = gm1*(have-thtave)
C UN removed      cave = max (cave, zero)                                
C       
C UN changed      cave = sqrt (cave)

      cave = sin (cave)
      uhat = nxhat*uave+nyhat*vave+nzhat*wave
      utilde = nyhat*wave-nzhat*vave
      vtilde = nzhat*uave-nxhat*wave
      wtilde = nxhat*vave-nyhat*uave

C     wavespeeds

      lamcm = uhat-cave
      lamu = uhat
      lamcp = uhat+cave

C     harten's entropy fix                                              
C     nonlinear waves                                                   
C UN changed     delta = nlefix * (abs (uhat) + cave)

      delta = nlefix*(uhat+cave)
      delta2 = delta**2
                                                                        
      lam2 = lamcm**2
      IF (lam2.gt.delta2) then
C UN changed                alamcm = abs (lamcm)

                alamcm = lamcm
      ELSE
                alamcm = half*(lam2+delta2)/delta2
      ENDIF
                                                                        
      lam2 = lamcp**2
      IF (lam2.gt.delta2) then
C UN changed                alamcp = abs (lamcp)

                alamcp = lamcp
      ELSE
                alamcp = half*(lam2+delta2)/delta2
      ENDIF

C     linear waves                                                      
C UN changed      delta = lefix * (abs (uhat) + cave)

      delta = lefix*(uhat+cave)
      delta2 = delta*delta
                                                                        
      lam2 = lamu**2
      IF (lam2.gt.delta2) then
C UN changed                alamu = abs (lamu)

                alamu = lamu
      ELSE
                alamu = half*(lam2+delta2)/delta2
      ENDIF

C     form mlam=0.5*(lam-|lam|)

      alamcm = 0.5d0*(lamcm-alamcm)
      alamcp = 0.5d0*(lamcp-alamcp)
      alamu = 0.5d0*(lamu-alamu)

C     conservative variables

      el = priml(1)*gm1inv+priml(2)*thetal
      rul = priml(2)*priml(3)
      rvl = priml(2)*priml(4)
      rwl = priml(2)*priml(5)
      er = primr(1)*gm1inv+primr(2)*thetar
      rur = primr(2)*primr(3)
      rvr = primr(2)*primr(4)
      rwr = primr(2)*primr(5)

C     change in conservative variables

      de = er-el
      dr = primr(2)-priml(2)
      dru = rur-rul
      drv = rvr-rvl
      drw = rwr-rwl

C     wave-strengths

      uddru = uave*dru+vave*drv+wave*drw
      omega = (gm1/cave)*(de+thtave*dr-uddru)
      nddru = nxhat*dru+nyhat*drv+nzhat*drw
                                                                        
      alp1 = half*(omega+uhat*dr-nddru)
      alp2 = (-nxhat)*omega+(nxhat*cave-utilde)*dr-nzhat*drv+nyhat*drw
      alp3 = (-nyhat)*omega+(nyhat*cave-vtilde)*dr-nxhat*drw+nzhat*dru
      alp4 = (-nzhat)*omega+(nzhat*cave-wtilde)*dr-nyhat*dru+nxhat*drv
      alp5 = half*(omega-uhat*dr+nddru)

C     multiply by wave-speeds

      alp1 = alamcm*alp1
      alp2 = alamu*alp2
      alp3 = alamu*alp3
      alp4 = alamu*alp4
      alp5 = alamcp*alp5

C     form right-eigenvector contribution

      alp15p = alp1+alp5
      alp15m = alp1-alp5
      mu = alp15p/cave+nxhat*alp2+nyhat*alp3+nzhat*alp4
                                                                        
      dss1 = mu*thtave+(cave*gm1inv*alp15p-uhat*alp15m)+utilde*alp2+vtil
     +de*alp3+wtilde*alp4
      dss2 = mu
      dss3 = mu*uave-nxhat*alp15m+nzhat*alp3-nyhat*alp4
      dss4 = mu*vave-nyhat*alp15m+nxhat*alp4-nzhat*alp2
      dss5 = mu*wave-nzhat*alp15m+nyhat*alp2-nxhat*alp3

C     form flux

      uhatl = nxhat*priml(3)+nyhat*priml(4)+nzhat*priml(5)
                                                                        
      flux(1) = nsize*((el+priml(1))*uhatl+dss1)
      flux(2) = nsize*(priml(2)*uhatl+dss2)
      flux(3) = nsize*(rul*uhatl+nxhat*priml(1)+dss3)
      flux(4) = nsize*(rvl*uhatl+nyhat*priml(1)+dss4)
      flux(5) = nsize*(rwl*uhatl+nzhat*priml(1)+dss5)
C$openad DEPENDENT(flux)

      END SUBROUTINE ad_roehf5
