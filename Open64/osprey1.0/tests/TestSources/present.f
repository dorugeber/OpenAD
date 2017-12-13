		 subroutine opt(reqarg, optarg)
		 integer, intent (out) :: reqarg
		 logical, optional, intent (in) :: optarg
		 if(present(optarg)) then
			 if(optarg) then
		            reqarg=1
		         else
		            reqarg=2
			 end if
		  else
			  reqarg=3
		  end if
		  end subroutine opt
		  
		  program main
                  interface
                     subroutine opt(reqarg, optarg)
		     integer, intent (out) :: reqarg
		     logical, optional, intent (in) :: optarg
                     end subroutine opt
                  end interface
		  integer ret1, ret2, ret3
		  call opt(ret1, .TRUE.)
		  call opt(ret2, .FALSE.)
		  call opt(ret3)
		  if((ret1.EQ.1).AND.(ret2.EQ.2).AND.(ret3.EQ.3)) then
			  print *, "OK"
		  else
			  print *, "failed"
		  end if
		  end program main
		  
			  