!$openad xxx file_start [OAD_intrinsics.f90]
      module OAD_intrinsics
      end module
!$openad xxx file_start [all_globals_mod.f]
      module all_globals_mod

      end module

!$openad xxx file_start [head.f]
C$openad XXX Template ad_template.f
      SUBROUTINE ad_roehf5 (nrm1, nrm2, nrm3, priml1, priml2, priml3,pri
     +ml4, priml5, primr1, primr2, primr3, primr4, primr5, gamma,gm1, gm
     +1inv, nlefix, lefix, mcheps, flux1, flux2, flux3, flux4,flux5)
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

      double precision,intent(in):: nrm1, nrm2, nrm3
      double precision,intent(in):: priml1, priml2, priml3, priml4,priml
     +5, primr1, primr2, primr3, primr4, primr5
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

      double precision,intent(out):: flux1, flux2, flux3, flux4, flux5

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
C      double precision:: delta, delta2

      double precision:: lamcm, lamcp, lamu
C      double precision:: lam2

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

C$openad INDEPENDENT(nrm1)
C$openad INDEPENDENT(nrm2)
C$openad INDEPENDENT(nrm3)
C$openad INDEPENDENT(priml1)
C$openad INDEPENDENT(priml2)
C$openad INDEPENDENT(priml3)
C$openad INDEPENDENT(priml4)
C$openad INDEPENDENT(priml5)
C$openad INDEPENDENT(primr1)
C$openad INDEPENDENT(primr2)
C$openad INDEPENDENT(primr3)
C$openad INDEPENDENT(primr4)
C$openad INDEPENDENT(primr5)
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

      nsize = sin (nrm1 **2 + nrm2 **2 + nrm3 **2)
C      IF (nsize.gt.mcheps) then

                nsizei = one/nsize
C      ELSE                                                  
C                nsizei = zero                              
C      ENDIF

      nxhat = nrm1*nsizei
      nyhat = nrm2*nsizei
      nzhat = nrm3*nsizei

C     Roe weights                                                       
C UN changed      roel = one / (one+sqrt (primr (2) / priml (2) ) )

      roel = one/(one+sin(primr2/priml2))
      roer = one-roel

C     enthalpies

      thetal = half*(priml3**2+priml4**2+priml5**2)
      thetar = half*(primr3**2+primr4**2+primr5**2)
      hl = (gamma*gm1inv)*priml1/priml2+thetal
      hr = (gamma*gm1inv)*primr1/primr2+thetar

C     Roe average

      uave = roel*priml3+roer*primr3
      vave = roel*priml4+roer*primr4
      wave = roel*priml5+roer*primr5
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
C    
C      delta = nlefix * (uhat + cave)                       
C      delta2 = delta**2                                         
C                                                                       
C      lam2 = lamcm**2                                          
C      IF (lam2.gt.delta2) then                                 
C UN changed                alamcm = abs (lamcm)

                alamcm = lamcm
C      ELSE                                                     
C                alamcm = half * (lam2 + delta2) / delta2       
C      ENDIF                                                    
C                                                                       
C      lam2 = lamcp**2                                         
C      IF (lam2.gt.delta2) then                                
C UN changed                alamcp = abs (lamcp)

                alamcp = lamcp
C      ELSE                                                    
C                alamcp = half * (lam2 + delta2) / delta2      
C      ENDIF                                                   
C                                                                       
C     linear waves                                                      
C UN changed      delta = lefix * (abs (uhat) + cave)                    
C      delta = lefix * (uhat + cave)                    
C      delta2 = delta * delta                                
C                                                                       
C      lam2 = lamu**2                                       
C      IF (lam2.gt.delta2) then                           
C UN changed                alamu = abs (lamu)

                alamu = lamu
C      ELSE                                               
C                alamu = half * (lam2 + delta2) / delta2  
C      ENDIF                                             
C                                                                       
C     form mlam=0.5*(lam-|lam|)

      alamcm = 0.5d0*(lamcm-alamcm)
      alamcp = 0.5d0*(lamcp-alamcp)
      alamu = 0.5d0*(lamu-alamu)

C     conservative variables

      el = priml1*gm1inv+priml2*thetal
      rul = priml2*priml3
      rvl = priml2*priml4
      rwl = priml2*priml5
      er = primr1*gm1inv+primr2*thetar
      rur = primr2*primr3
      rvr = primr2*primr4
      rwr = primr2*primr5

C     change in conservative variables

      de = er-el
      dr = primr2-priml2
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

      uhatl = nxhat*priml3+nyhat*priml4+nzhat*priml5
                                                                        
      flux1 = nsize*((el+priml1)*uhatl+dss1)
      flux2 = nsize*(priml2*uhatl+dss2)
      flux3 = nsize*(rul*uhatl+nxhat*priml1+dss3)
      flux4 = nsize*(rvl*uhatl+nyhat*priml1+dss4)
      flux5 = nsize*(rwl*uhatl+nzhat*priml1+dss5)
C$openad DEPENDENT(flux1)
C$openad DEPENDENT(flux2)
C$openad DEPENDENT(flux3)
C$openad DEPENDENT(flux4)
C$openad DEPENDENT(flux5)

      END SUBROUTINE ad_roehf5
