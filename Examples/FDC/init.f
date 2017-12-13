      subroutine init(nx1,nx2,x0)
      double precision  x0(nx1*nx2)
      integer nx1,nx2
      integer i,j,k
      double precision  hx1,hx2,xx2,xx1
      double precision zero,one,two,three,four
      parameter(zero=0.0d0,one=1.0d0,two=2.0d0,three=3.0d0,four=4.0d0)

      hx1 = one/dble(nx1 + 1)
      hx2 = one/dble(nx2 + 1)
      
      xx2 = hx2
      do i = 1, nx2
         xx1 = hx1
         do j = 1, nx1
            k = (i - 1)*nx1 + j
            x0(k) = - xx1*(one - xx1)*xx2*(one - xx2)
            xx1 = xx1 + hx1
         end do
         xx1 = hx1
         xx2 = xx2 + hx2
      end do
      end subroutine
