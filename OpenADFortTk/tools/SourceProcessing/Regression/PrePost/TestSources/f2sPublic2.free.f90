MODULE m
PRIVATE
PUBLIC  :: foo

CONTAINS

FUNCTION foo(i) RESULT(status)
        INTEGER i
        INTEGER status
        status=1
        return;
END FUNCtion

end module

program p 
use m
i=2
j=foo(i)
end 
