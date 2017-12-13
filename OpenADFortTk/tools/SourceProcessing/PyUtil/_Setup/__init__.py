import sys,os.path
__mypath = sys.path[0]

__libpath = os.path.normpath(os.path.join(sys.path[0],'..'))

# print "PyUtil/_Setup inserting %s, cur path = %s" %(__libpath,sys.path)

if __libpath not in sys.path:
    sys.path.insert(1,__libpath)


