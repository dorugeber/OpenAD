'''
Buffered iterator -- try to avoid itertool.chain for backtracking,
as I think the closure captures the values
'''
import copy as c

class buf_iter(object):
    def __init__(self,iterable):
        self.iterable   = iter(iterable)
        self.lookahead  = []
        self.myCounter = 0

    def putback(self,lst):
        ll = c.copy(lst)
        ll.reverse()
        self.lookahead.extend(ll)
        self.myCounter -= len(ll)

        return self

    def next(self):
        self.myCounter += 1
        if not self.lookahead:
            return self.iterable.next()
        v = self.lookahead.pop()
        return v

    def __iter__(self):
        return self
