program nullifyTest
  double precision, pointer :: x
  nullify(x)
  if (.not.associated(x)) then 
    print *, 'OK' 
  end if
end program
