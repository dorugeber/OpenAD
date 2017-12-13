module OAD_intrinsics
end module
program scanStringConst
      use OAD_intrinsics
      character(len=1),dimension(0:27),parameter :: char_array = (/'0','1','2','&
     &3','4','5','6','7','8','9','a','b','c','d','e','f',' ',',','r','*','$','&'&
     &,'+','-','z','.','o','p'/)

      print *,char_array(4)
      print *,' <APPLET \'
      print *,'  code = "ptolemy.plot.compat.PxgraphApplet" \'
      print *,'  code = "ptolemy.plot.PlotApplet" \'
      print *,'  code = "javapeno.javapeno"\'

 10351       format(5x,'nfst',t15,'=',t20,i5, t30,'multigroup calculation option&
     & in upper energy range (UMR):'/ t32, '0/1/2/3   = sn/diffusion/homogeneous&
     &/zonewise homogeneous (0)'/ 5x,'nthr',t15,'=',t20,i5, t30,'multigroup calc&
     &ulation option in lower energy range (LMR):'/ t32,'0/1/2/3   = sn/diffusio&
     &n/homogeneous/zonewise homogeneous (0)'/ 5x,'npxs',t15,'=',t20,i5,t30,'poi&
     &ntwise calculation option:'/ t32,'<=0/1/2/3/4/5 = no pointwise: do multigr&
     &oup as in nfst/sn/coll. prob./'/ t32,'  homogeneous/zonewise homogeneous/z&
     &onewise two-region (1)'/ 5x,'isvar',t15,'=',t20,i5, t30,'multigroup source&
     & & cross section linearization option :'/ t32,'0/1/2/3 = none/linearize so&
     &urce/linearize group cross sections/both (3)'/ 5x,'iscti',t15,'=',t20,i5,t&
     &30,'inelastic scattering order (1)'/ 5x,'nmf6',t15,'=',t20,i5, t30,'inelas&
     &tic scattering option, default (0)  '/ t32,'-1/0/1 = no inelastic/discrete&
     & level only/discrete + continuum ')


 10400       format(//,5x,'2 $ $',5x,'EDITTING AND OTHER OPTIONS (13)'//, 5x,'ip&
     &rt',t15,'=',t20,i5,t30,'-3/-2/-1/n = mixture cross', ' sections print opti&
     &ons: ',/,t32, 'none/print P.W. energy & x-sections on file lsf2', '/print &
     &1-D M.G. x-sections',/,t32, '/print nth p (p0,p1..) M.G. x-section matrix &
     &(-3)'/, 5x,'id1',t15,'=',t20,i5,t30,'-1/0/1/2/3/4 = flux print options:', &
     &/,t33,'none/print multigroup flux/also punch multigroup flux',/, t33,'/pun&
     &ch pointwise flux (tab1 format)/punch both/also print', ' multigroup momen&
     &ts (-1)'/, 5x,'ipbt',t15,'=',t20,i5,t30, '0/1 = none/group summary table p&
     &rint (0)'/, 5x,'iqm',t15,'=',t20,i5,t30,'volumetric sources (0/n=no/yes)'/&
     &, 5x,'ipm',t15,'=',t20,i5,t30,'boundary sources (0/n=no/yes)'/, 5x,'ipn',t&
     &15,'=',t20,i5,t30,'0/1/2 diff. coef. param (2)'/, 5x,'idfm',t15,'=',t20,i5&
     &,t30,'0/1 = none/density factors (0)'/, 5x,'ixprt',t15,'=',t20,i5,t30,'0/1&
     & = minimum print/regular print (0)')

end program scanStringConst
