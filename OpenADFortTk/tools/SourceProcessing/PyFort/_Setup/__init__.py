import sys,os
__mypath = sys.path[0]

__libpath = os.path.normpath(os.path.join(sys.path[0],'..'))

if __libpath not in sys.path:
    sys.path.insert(1,__libpath)
#    print "PyFort/_Setup inserting %s, giving path = %s" %(__libpath,sys.path)
