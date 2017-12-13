c
c     weight matices are all diagonal for now
c
      common /weight_factors/ wf_depth, wf_eta, wf_u, wf_v,
     &     wf_zonal_transport, wf_lapldepth, wf_graddepth
      real wf_depth, wf_eta, wf_u, wf_v, wf_lapldepth, wf_graddepth
      real wf_zonal_transport
c
      common /weights/ weight_depth, weight_eta, weight_u, weight_v, 
     &     weight_lapldepth, weight_graddepth, weight_zonal_transport
      real weight_depth(nx,nx,ny,ny)
      real weight_eta(nx,ny)
      real weight_u(nx,ny)
      real weight_v(nx,ny)
      real weight_lapldepth(nx,ny)
      real weight_graddepth(nx,ny)
      real weight_zonal_transport
c
