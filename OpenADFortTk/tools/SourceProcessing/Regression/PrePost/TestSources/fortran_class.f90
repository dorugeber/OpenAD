program sClass

  type base
     integer::i
  end type base
  
  type,extends(base)::child
     integer::j
  end type child
  
  type(child),target::child1=child(4,6)
  type(base), target::base1=base(7)
  ! declare an item that could contain any extensible derived type
  ! or intrinsic type
  class(*),allocatable::anyThing
  
  ! declare basePtr as a polymorphic item with declared type base,
  ! could have run time type of base or child
  class(base),pointer::basePtr
  
  ! set basePtr to point to an item of type child
  basePtr=>child1
  call printAny(basePtr)
  
  ! set basePtr to point to an item of type base
  basePtr=>base1
  call printAny(basePtr)
  
  ! allocate an integer item
  allocate(anyThing, source=base1%i)
  call printAny(anyThing)
  
contains
  
  subroutine printAny(printItem)
    ! declare a dummy arg of unlimited polymorphic, can point
    ! to any extensible derived type or intrinsic type
    class(*)::printItem
    
    select type(item=>printItem)
    type is (base)
       print*,' base item is ',item
       
    type is (child)
       print*,' child item is ', item
       
    type is (integer)
       print*,' integer item is ',item
    end select
  end subroutine printAny
end program sClass
