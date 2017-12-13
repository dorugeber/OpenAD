      subroutine foo(x,y,z)
          type(OpenAD_type) x,xx1
          real y,z
          use w2f__types

          y = z * z
          __value__(x) = z * __value__(x)
          call goo(y,__deriv__(x),z)
      end
