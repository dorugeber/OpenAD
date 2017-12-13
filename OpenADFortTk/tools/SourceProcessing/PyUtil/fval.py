'wrap up a function value, so it can be stored in an object'

class Fval(object):
    '''wrap up a function value so that it will not be converted
    to a method
    '''
    def __init__(self,fval):
        self.__class__.__call__ = staticmethod(fval)

