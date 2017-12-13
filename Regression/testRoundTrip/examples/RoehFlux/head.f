c$openad XXX Template ad_template.f
      SUBROUTINE ad_roehf5 (nrm, priml, primr, gamma, gm1, gm1inv, 
     &nlefix, lefix, mcheps, flux) 
!     !*****************************************************************
!     !  SUBROUTINE: AD_ROE_HARTEN_FLUX5_EXP                            
!     !      AUTHOR: Shaun Forth                                        
!     !        DATE: 050997                                             
!     !   COPYRIGHT: RMCS, Cranfield University 1997                    
!     !    ABSTRACT: For MUSCL interpolated cell face primitive values  
!     !              calculates explicit euler flux for 3-D inviscid gas
!     !              turbulence  model (i.e. 5 variables).              
!     !                                                                 
!     !    MODIFIED: Uwe Naumann, Feb 04                                       
!     !                                                                 
!     !*****************************************************************
      IMPLICIT none               
!     AUGUMENTS(in):                                                    
!     nrm    : cell face normal                                         
!     priml  : primitive variables left of cell face                    
!     primr  : primitive variables right of cell face                   
      double precision,intent(in):: nrm (3)
      double precision,intent(in):: priml (5), primr (5)                      
!     gamma  : ratio of specific heats                                  
!     gm1    : gamma - 1                                                
!     gm1inv : 1/gm1                                                    
!     nlefix : non-linear entropy fix parameter                         
!     lefix  : linear entropy fix parameter                             
!     mcheps : machine epsilon                                          
      double precision,intent(in):: gamma, gm1, gm1inv, nlefix, lefix, 
     &mcheps 
                                                                        
                                                                        
!     AUGUMENTS(out):                                                   
!     flux  : inviscid flux                                             
      double precision,intent(out):: flux (5)                                
                                                                        
!     PARAMETERS                                                        
      real:: one, half, zero                                    
      PARAMETER (one = 1.0d0, half = 0.5d0, zero = 0.0d0)       
                                                                        
!     LOCAL VARIABLES                                                   
!     independent of flow variables                                     
!     nsize,nsizei      : size of normal and reciprocal                 
!     nxhat,nyhat,nzhat : components of normalised normal               
      double precision:: nsize, nsizei, nxhat, nyhat, nzhat                 
                                                                        
!     dependent on flow variables                                       
!     roel,roer  : Roe averaging weights                                
!     thetal,thetar :  |ul^2| |ur|^2                                    
!     hl,hr         : left and right specific enthalpies                
!     uave,vave,wave,have,cave : Roe average values                     
!     uhat,thtave              : uave.nhat, 0.5|uave^2|                 
!     utilde,vtilde,wtilde     : nhat x u                               
!     delta,delta2             : size of entropy fix                    
!     lamcm,lamcp,lamu         : raw eigenvalues/wavespeeds             
!     lam2                     : eigenvalue**2                          
!     alamcm,alamcp,alamu      : entropy fixed eigenvalues/wavespeeds   
!     el,rul,rvl,rwl           : left conservative variables            
!     er,rur,rvr,rwr           : right conservative variables           
!     de,dr,dru,drv,drw       :  change in conservative variables       
!     uddru,omega,nddru        : intermediate wave-strength vars.       
!     alp1,alp2,alp3,alp4,alp5 : wave-strengths                         
!     alp15p,alp15m            : alam1*alp1+/-alam5*alp5                
!     mu                       : used for dissipation vector            
!     dss1,dss2,dss3,dss4,dss5 : dissipation vector                     
!     uhatl                    : u.nhat left side of face               
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

c$openad INDEPENDENT(nrm)
c$openad INDEPENDENT(priml)
c$openad INDEPENDENT(primr)
c$openad INDEPENDENT(gamma)
c$openad INDEPENDENT(gm1)
c$openad INDEPENDENT(gm1inv)
c$openad INDEPENDENT(nlefix)
c$openad INDEPENDENT(lefix)
c$openad INDEPENDENT(mcheps)

!     CODE                                                              
                                                                        
!     set normal quantities                                             
! UN changed      nsize = sqrt (nrm (1) **2 + nrm (2) **2 + nrm (3) **2)   
      nsize = sin (nrm (1) **2 + nrm (2) **2 + nrm (3) **2)   
      IF (nsize.gt.mcheps) then                               
                nsizei = one / nsize                         
      ELSE                                                  
                nsizei = zero                              
      ENDIF                                               
      nxhat = nrm (1) * nsizei                           
      nyhat = nrm (2) * nsizei                         
      nzhat = nrm (3) * nsizei                        
                                                                        
!     Roe weights                                                       
! UN changed      roel = one / (one+sqrt (primr (2) / priml (2) ) )  
      roel = one / (one+sin (primr (2) / priml (2) ) )  
      roer = one-roel                                   
                                                                        
!     enthalpies                                                        
      thetal = half * (priml (3) **2 + priml (4) **2 + priml (5) **2) 
      thetar = half * (primr (3) **2 + primr (4) **2 + primr (5) **2) 
      hl = (gamma * gm1inv) * priml (1) / priml (2) + thetal          
      hr = (gamma * gm1inv) * primr (1) / primr (2) + thetar          
                                                                        
!     Roe average                                                       
      uave = roel * priml (3) + roer * primr (3)                      
      vave = roel * priml (4) + roer * primr (4)                      
      wave = roel * priml (5) + roer * primr (5)                      
      have = roel * hl + roer * hr                                    
      thtave = half * (uave**2 + vave**2 + wave**2)                   
      cave = gm1 * (have-thtave)                                      
! UN removed      cave = max (cave, zero)                                      
! UN changed      cave = sqrt (cave)                                          
      cave = sin (cave)                                             
      uhat = nxhat * uave+nyhat * vave+nzhat * wave                  
      utilde = nyhat * wave-nzhat * vave                             
      vtilde = nzhat * uave-nxhat * wave                             
      wtilde = nxhat * vave-nyhat * uave                             
                                                                        
!     wavespeeds                                                        
      lamcm = uhat - cave                                           
      lamu = uhat                                                  
      lamcp = uhat + cave                                         
                                                                        
!     harten's entropy fix                                              
!     nonlinear waves                                                   
! UN changed     delta = nlefix * (abs (uhat) + cave)                       
      delta = nlefix * (uhat + cave)                       
      delta2 = delta**2                                         
                                                                        
      lam2 = lamcm**2                                          
      IF (lam2.gt.delta2) then                                 
! UN changed                alamcm = abs (lamcm)                           
                alamcm = lamcm                           
      ELSE                                                     
                alamcm = half * (lam2 + delta2) / delta2       
      ENDIF                                                    
                                                                        
      lam2 = lamcp**2                                         
      IF (lam2.gt.delta2) then                                
! UN changed                alamcp = abs (lamcp)                          
                alamcp = lamcp                          
      ELSE                                                    
                alamcp = half * (lam2 + delta2) / delta2      
      ENDIF                                                   
                                                                        
!     linear waves                                                      
! UN changed      delta = lefix * (abs (uhat) + cave)                    
      delta = lefix * (uhat + cave)                    
      delta2 = delta * delta                                
                                                                        
      lam2 = lamu**2                                       
      IF (lam2.gt.delta2) then                           
! UN changed                alamu = abs (lamu)                       
                alamu = lamu                       
      ELSE                                               
                alamu = half * (lam2 + delta2) / delta2  
      ENDIF                                             
                                                                        
!     form mlam=0.5*(lam-|lam|)                                         
      alamcm = 0.5d0 * (lamcm - alamcm)                             
      alamcp = 0.5d0 * (lamcp - alamcp)                             
      alamu = 0.5d0 * (lamu - alamu)                                
                                                                        
!     conservative variables                                            
      el = priml (1) * gm1inv + priml (2) * thetal                 
      rul = priml (2) * priml (3)                                  
      rvl = priml (2) * priml (4)                                  
      rwl = priml (2) * priml (5)                                  
      er = primr (1) * gm1inv + primr (2) * thetar                 
      rur = primr (2) * primr (3)                                
      rvr = primr (2) * primr (4)                                  
      rwr = primr (2) * primr (5)                                 
                                                                        
!     change in conservative variables                                  
      de = er - el                                                       
      dr = primr (2) - priml (2)                                         
      dru = rur - rul                                                    
      drv = rvr - rvl                                                    
      drw = rwr - rwl                                                    
                                                                        
!     wave-strengths                                                    
      uddru = uave * dru + vave * drv + wave * drw                       
      omega = (gm1 / cave) * (de+thtave * dr - uddru)                    
      nddru = nxhat * dru + nyhat * drv + nzhat * drw                    
                                                                        
      alp1 = half * (omega + uhat * dr - nddru)                          
      alp2 = ( - nxhat) * omega + (nxhat * cave-utilde) * dr - nzhat * 
     &drv + nyhat * drw                                                 
      alp3 = ( - nyhat) * omega + (nyhat * cave-vtilde) * dr - nxhat * 
     &drw + nzhat * dru                                                 
      alp4 = ( - nzhat) * omega + (nzhat * cave-wtilde) * dr - nyhat * 
     &dru + nxhat * drv                                                 
      alp5 = half * (omega - uhat * dr + nddru)                          
                                                                        
!     multiply by wave-speeds                                           
      alp1 = alamcm * alp1                                               
      alp2 = alamu * alp2                                                
      alp3 = alamu * alp3                                                
      alp4 = alamu * alp4                                                
      alp5 = alamcp * alp5                                               
                                                                        
!     form right-eigenvector contribution                               
      alp15p = alp1 + alp5                                               
      alp15m = alp1 - alp5                                               
      mu = alp15p / cave+nxhat * alp2 + nyhat * alp3 + nzhat * alp4      
                                                                        
      dss1 = mu * thtave+ (cave * gm1inv * alp15p - uhat * alp15m)    
     &+ utilde * alp2 + vtilde * alp3 + wtilde * alp4                   
      dss2 = mu                                                          
      dss3 = mu * uave-nxhat * alp15m + nzhat * alp3 - nyhat * alp4      
      dss4 = mu * vave-nyhat * alp15m + nxhat * alp4 - nzhat * alp2      
      dss5 = mu * wave-nzhat * alp15m + nyhat * alp2 - nxhat * alp3      
                                                                        
!     form flux                                                         
      uhatl = nxhat * priml (3) + nyhat * priml (4) + nzhat * priml (5)  
                                                                        
      flux (1) = nsize * ( (el + priml (1) ) * uhatl + dss1)             
      flux (2) = nsize * (priml (2) * uhatl + dss2)                      
      flux (3) = nsize * (rul * uhatl + nxhat * priml (1) + dss3)        
      flux (4) = nsize * (rvl * uhatl + nyhat * priml (1) + dss4)        
      flux (5) = nsize * (rwl * uhatl + nzhat * priml (1) + dss5)        
c$openad DEPENDENT(flux)                                                                        
      END SUBROUTINE ad_roehf5
