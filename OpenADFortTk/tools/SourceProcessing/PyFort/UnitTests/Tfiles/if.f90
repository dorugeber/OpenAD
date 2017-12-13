  subroutine head(x,y)
  interface
    subroutine foo(x,y)
      double precision x
      double precision y
    end subroutine
    function foo2(a)
      implicit integer (a,f)
    end
  end interface

  y = x * x
  
  end subroutine head
