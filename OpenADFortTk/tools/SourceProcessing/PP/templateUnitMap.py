from PyFort.fortUnit import fortUnitIterator
from PyUtil.errors import UserError

_ourTemplateUnitMap={}

def getTemplateUnit(templateFile):
    if templateFile in _ourTemplateUnitMap:
        return _ourTemplateUnitMap[templateFile]
    else:
        sanityCount=0
        rUnit=None
        for aUnit in fortUnitIterator(templateFile,False):
            if sanityCount==0:
                _ourTemplateUnitMap[templateFile]=aUnit
                rUnit=aUnit
            else:
                if (aUnit.uinfo):
                    raise UserError('template file '+templateFile+' should contain one compile unit but has at least '+_ourTemplateUnitMap[templateFile].uinfo.name+" and "+aUnit.uinfo.name)
            sanityCount+=1
        return _ourTemplateUnitMap[templateFile]
            
