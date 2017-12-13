c
c     data arrays
c
      common /data/ eta_data, u_data, v_data, depth_data,
     &     eta_data_time, zonal_transport_data, nedt
      integer nedt, nedtmax
      parameter ( nedtmax = 1000 ) 
      real eta_data(nx,ny), u_data(nx,ny), v_data(nx,ny)
      real depth_data(nx,ny)
      real eta_data_time(nedtmax)
      real zonal_transport_data
c
      
