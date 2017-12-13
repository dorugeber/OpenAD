! front end problem with multiple renames

module a
	integer longName
end module

module b
	use a, only: s => longName
end module 

module c
	use a, only: s => longName
end module 

program multRename
	use a
	longName=1
	if (longName==1) then 
	print *, 'OK'
	else 
	print *, 'FAILED'
	end if
end program
