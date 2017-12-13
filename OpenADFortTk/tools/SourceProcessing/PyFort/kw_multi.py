'''
Some Fortran keywords can be written with or without spaces
This data structure lists the double keywords and the associated
single keyword
'''

_kw2 = (('block','data'),
        ('do','while'),
        ('double','complex'),
        ('double','precision'),
        ('else','if'),
        ('end','blockdata'),
        ('endblock','data'),
        ('end','do'),
        ('end','file'),
        ('end','forall'),
        ('end','function'),
        ('end','if'),
        ('end','interface'),
        ('end','module'),
        ('end','program'),
        ('end','select'),
        ('end','subroutine'),
        ('end','type'),
        ('end','where'),
        ('go','to'),
        ('in','out'),
        ('select','case'),
        ('case','default'),
        ('select','type'),
        ('type','is'),
        ('class','is'),
        )
_kw3 = (('end','block','data'),
        )

kw2 = dict()
for pair in _kw2:
    kw2[pair] = ''.join(pair)

kw3 = dict()
for group in _kw3:
    kw3[group] = ''.join(group)
