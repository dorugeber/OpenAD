!------------------------------------------------------------------
program Poisson_Equation
!------------------------------------------------------------------

   integer, parameter :: N_X = 500         ! grid-points in X-direction
   integer, parameter :: N_Y = N_X         ! grid-points in Y-direction
   integer, parameter :: N   = N_X * N_Y   ! total number of gridpoints

   double precision, dimension (N_X, N_Y) :: U  ! solution vector
   double precision, dimension (N_X, N_Y) :: F  ! right hand side of
                                                !   discretized equation

   double precision, dimension (N_X, N_Y) :: r   ! residual vector
   double precision, dimension (N_X, N_Y) :: p   ! search direction vector
   double precision, dimension (N_X, N_Y) :: Ap  ! product vector

!HPF$ DISTRIBUTE (*, BLOCK) :: U,F,r, p, Ap

   double precision                :: epsilon
   double precision                :: ri_dot_ri_old
   double precision                :: ri_dot_ri_new
   double precision                :: r_old
   double precision                :: r_new
   double precision                :: pi_dot_Api
   double precision                :: alpha
   double precision                :: beta
   integer                         :: max_iter, loops
   logical                         :: converged

   double precision                 ::  delta_x
   double precision                 ::  T

   real                          :: T_1, T_2 

   loops = 0

   call Define_F

   T_1 = SECNDS(0.0) ! zero the timer

   max_iter = 1000
   epsilon = 1.0D-10
   converged = .false.

   u = 0.0D0
   r = F
   p = r
   ri_dot_ri_old = SUM(r * r)      
   r_old = sqrt(ri_dot_ri_old)

   do iter = 1, max_iter

      delta_x = 1.0d0 / dble(N_X - 1)

      forall (i = 1 : N_X)
         Ap(i,1)    = 0.0d0 ! south
         Ap(i, N_Y) = 0.0d0 ! north
      end forall

      forall (j = 1 : N_Y)
         Ap(N_X, j) = 0.0d0 ! east
         Ap(1, j)   = 0.0d0 ! west
      end forall

      T = 1.0d0 / (delta_x * delta_x)

      forall (i = 2 : N_X-1, j = 2: N_Y-1)
	Ap(i,j) = (-p(i,j-1)-p(i-1,j)+4.0D0*p(i,j)-p(i+1,j)-p(i,j+1))*T
      end forall

      pi_dot_Api = SUM(p * Ap)                                  
      alpha = ri_dot_ri_old / pi_dot_Api                      
      u = u + alpha*p
      r = r - alpha*AP                                        
      ri_dot_ri_new = SUM(r * r)                                
      r_new = sqrt(ri_dot_ri_new)                             
                        
      beta = (r_new * r_new) / (r_old * r_old)             
      r_old = r_new                                        
      ri_dot_ri_old = ri_dot_ri_new                        
      p = r + beta*p                                       
      loops = loops + 1

   enddo

   T_2 = SECNDS(T_1)

      U_Max  = MAXVAL(U)
      U_Min  = MINVAL(U)
      U_Norm =    SUM(U)                   
                                                              
      Range = U_Max - U_Min

      write (*, *) '============================================'
      write (*, *) 'HPF'
      write (*, *) '============================================'
      write (*, *) 'N          => ', n
      write (*, *) 'Tolerance  => ', epsilon
      write (*, *) 'U(Max)     => ', U_Max
      write (*, *) 'U(Min)     => ', U_Min
      write (*, *) '| U |      => ', U_Norm
      write (*, *) 'Iterations => ', loops
      write (*, *) 'Time       => ', t_2
      write (*, *) '============================================'
      write (*, *) '   '

contains
!-------

   subroutine Define_F

!
!  - Uxx - Uyy = 1 on [0,1] X [0, 1] ; U = 0 on boundary
!

      F = 1.0 ! set RHS vector 

      forall (i = 1 : N_X)
         F(i,1)    = 0.0d0 ! correct RHS for south boundary
         F(i, N_Y) = 0.0d0 ! correct RHS for north boundary
      end forall

      forall (j = 1 : N_Y)

         F(N_X, j)  = 0.0d0 ! correct RHS for east boundary
         F(1, j)    = 0.0d0 ! correct RHS for west boundary

      end forall

   end subroutine Define_F

end Program Poisson_Equation
