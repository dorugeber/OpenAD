function bla(xbla)
  real :: xbla,bla
  bla=2*xbla
end function 

function foo(f,xfoo) 
  real:: xfoo,foo
  interface 
   function f(xf)
     real :: xf,f
   end function 
  end interface 
  foo=f(xfoo)
end function 

! the function that takes another function as dummy
function bar(ff,f,xbar)
  real::xbar,bar
  interface 
    function ff(f,xff) 
      real:: xff,ff
      interface 
        function f(xf)
          real :: xf,f
        end function 
      end interface 
    end function 
  end interface 
  interface 
     function f(xf)
       real :: xf,f
     end function
  end interface
  bar=ff(f,xbar)
end function bar

program main 
  real:: x,y
  external bar, foo, bla
  x=3.5
  y=bar(foo,bla,x)
  print *,y
end program
