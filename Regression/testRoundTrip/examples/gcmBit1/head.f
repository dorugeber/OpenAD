c$openad XXX Template ad_template.f
      SUBROUTINE FIND_BULKMOD(
     I      locPres, tFld, sFld,
     O      bulkMod)

      Real*8 eosJMDCKFw(5), eosJMDCKSw(7), eosJMDCKP(14)

      Real*8 locPres(2,2)
      Real*8 tFld(2,2,2,2,2)
      Real*8 sFld(2,2,2,2,2)
      Real*8 bulkMod(2,2)
      INTEGER i,j,k,bi,bj
      Real*8 bMfresh, bMsalt, bMpres
      Real*8 t, t2, t3, t4, s, s3o2, p, p2
      real*8 SItoBar

      eosJMDCKFw(1)=19659.33 
      eosJMDCKFw(2)=144.4304 
      eosJMDCKFw(3)=-1.706103 
      eosJMDCKFw(4)=0.009648704 
      eosJMDCKFw(5)=-4.190253e-05

      eosJMDCKSw(1)=52.84855
      eosJMDCKSw(2)=-0.3101089 
      eosJMDCKSw(3)=0.006283263 
      eosJMDCKSw(4)=-5.084188e-05 
      eosJMDCKSw(5)=0.388664 
      eosJMDCKSw(6)=0.00908583 
      eosJMDCKSw(7)=-0.0004619924

      eosJMDCKP(1)=3.186519 
      eosJMDCKP(2)=0.02212276 
      eosJMDCKP(3)=-0.0002984642 
      eosJMDCKP(4)=1.956415e-06 
      eosJMDCKP(5)=0.006704388 
      eosJMDCKP(6)=-0.0001847318 
      eosJMDCKP(7)=2.059331e-07 
      eosJMDCKP(8)=0.0001480266 
      eosJMDCKP(9)=0.0002102898 
      eosJMDCKP(10)=-1.202016e-05 
      eosJMDCKP(11)=1.39468e-07 
      eosJMDCKP(12)=-2.040237e-06 
      eosJMDCKP(13)=6.128773e-08 
      eosJMDCKP(14)=6.207323e-10

      k=1
      bi=1
      bj=1
      SItoBar=1.0
      DO j=1,2
         DO i=1,2
            t  = tFld(i,j,k,bi,bj) 
            t2 = t*t
            t3 = t2*t
            t4 = t3*t
            s  = sFld(i,j,k,bi,bj)
          IF ( s .GT. 0.d0 ) THEN
            s3o2 = s*SQRT(s)
          ELSE
            s    = 0.d0
            s3o2 = 0.d0
          ENDIF
            p = locPres(i,j)*SItoBar
            p2 = p*p
            bMfresh = 
     &             eosJMDCKFw(1)
     &           + eosJMDCKFw(2)*t
     &           + eosJMDCKFw(3)*t2
     &           + eosJMDCKFw(4)*t3
     &           + eosJMDCKFw(5)*t4
            bMsalt =
     &         s*( eosJMDCKSw(1)
     &           + eosJMDCKSw(2)*t
     &           + eosJMDCKSw(3)*t2
     &           + eosJMDCKSw(4)*t3
     &           )
     &    + s3o2*( eosJMDCKSw(5)
     &           + eosJMDCKSw(6)*t
     &           + eosJMDCKSw(7)*t2
     &           )
            bMpres = 
     &         p*( eosJMDCKP(1)
     &           + eosJMDCKP(2)*t
     &           + eosJMDCKP(3)*t2
     &           + eosJMDCKP(4)*t3
     &           )
     &     + p*s*( eosJMDCKP(5)
     &           + eosJMDCKP(6)*t
     &           + eosJMDCKP(7)*t2
     &           )
     &      + p*s3o2*eosJMDCKP(8)
     &      + p2*( eosJMDCKP(9)
     &           + eosJMDCKP(10)*t
     &           + eosJMDCKP(11)*t2
     &           )
     &    + p2*s*( eosJMDCKP(12)
     &           + eosJMDCKP(13)*t
     &           + eosJMDCKP(14)*t2
     &           )
            bulkMod(i,j) = bMfresh + bMsalt + bMpres
         ENDDO
      ENDDO
      RETURN 
      END

c$openad XXX Template ad_template.f
      subroutine head(x,y)
      double precision, dimension(32) :: x
      double precision, dimension(4) :: y
      Real*8 locPres(2,2)
      Real*8 tFld(2,2,2,2,2)
      Real*8 sFld(2,2,2,2,2)
      Real*8 bulkMod(2,2)
      integer i,j,k,l,m,xcount
c$openad INDEPENDENT(x)
      xcount=1
      DO i=1,2
         DO j=1,2
            locpres(i,j)=2*x(xcount)
            DO k=1,2
               DO l=1,2
                  DO m=1,2
                     tFld(i,j,k,l,m)=3*x(xcount)
                     sFld(i,j,k,l,m)=4*x(xcount)
                     xcount=xcount+1
                  end do
               end do
            end do
         end do
      end do
      call FIND_BULKMOD(
     &     locPres, tFld, sFld,
     &     bulkMod)
      DO i=1,2
         DO j=1,2
            y((i-1)*2+j)=bulkmod(i,j)
         end do
      end do
c$openad DEPENDENT(y)
      end subroutine

