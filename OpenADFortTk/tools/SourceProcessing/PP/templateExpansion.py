from _Setup import *

from PyUtil.debugManager import DebugManager
import PyFort.fortStmts as fs
from PyFort.fortUnit import fortUnitIterator
import re,string
from PyFort.fortScan import scan1
from PP.templateUnitMap import getTemplateUnit

# Handles template expansion
class TemplateExpansion(object):

    # set something here for the unit tests
    # set default template file here
    _templateFile = 'ad_template.f'

    @staticmethod
    def setTemplateFile(templateFile):
        TemplateExpansion._templateFile = templateFile

    def __init__(self,aUnit):
        self.__myUnit = aUnit
        self.__myNewDecls = []
        self.__myNewExecs = []
        # the current unit being inserted from the inline file (reverse mode)
        self.__inlineUnit = None

    # appends non-empty/non-null statements from oldList to newList
    def __appendNonEmptyStmt(self,oldList,newList):
        newList.extend(filter(lambda l:l is not None and len(l.rawline)!=0,oldList))

    # This function inserts the exec statements into self.__myNewExecs
    # in the order determined by the template file
    # PARAMS:
    # aUnit: a unit from the template file
    # Decls: Declaration statements from the original input file
    #  -- grouped by pragma number
    # (all decls in pragma 1 -- specified by Begin Replacement & 
    #  End Replacement comments --  in Decls[1])
    def __expandTemplateDecls(self,aUnit,Decls):
        pat = re.compile(
            "[ ]*[!][ ]*[$]template[_]pragma[_]declarations",
            re.IGNORECASE)
        for aDecl in aUnit.decls:
            if aDecl.is_comment():
                match = pat.search(aDecl.rawline)
                if match:
                    newStmt = fs.Comments(aDecl.rawline[:match.start()])
                    self.__myNewDecls.append(newStmt)
                    # return to input
                    if len(Decls) > 0:
                        self.__appendNonEmptyStmt(Decls[0],self.__myNewDecls)
                        Decls[0] = None
                    # continue template
                    newStmt = fs.Comments(aDecl.rawline[match.end():])
                    self.__myNewDecls.append(newStmt)
                    continue
            self.__myNewDecls.append(aDecl)

        if Decls[0] != None:
            self.__appendNonEmptyStmt(Decls[0],self.__myNewDecls)
        i = 1
        while i < len(Decls):
            self.__appendNonEmptyStmt(Decls[i],self.__myNewDecls)
            i += 1        

    # This function inserts the exec statements into self.__myNewExecs
    # in the order determined by the template file
    # PARAMS:
    # aUnit: a unit from the template file
    # Execs: Exec statements from the original input file
    #  -- grouped by pragma number 
    # (all execs in pragma 1 -- specified by Begin Replacement & 
    #  End Replacement comments --  in Execs[1])
    def __expandTemplateExecs(self, aUnit, Execs):
        if len(Execs) > 0:
            self.__appendNonEmptyStmt(Execs[0],self.__myNewExecs)
        execRepNum = 0
        firstIter = True
        pragma_pat = re.compile(
            "[!][ ]*[$]placeholder[_]pragma[$][ ]+id[=]",
            re.IGNORECASE)
        num_pat = re.compile("[0-9]+")
        for anExecStmt in aUnit.execs:
            if anExecStmt.is_comment():
                match = pragma_pat.search(anExecStmt.rawline)
                if match:
                    stmt = anExecStmt.rawline[:match.start()]
                    if len(stmt.strip()) != 0:
                        newStmt = self.__insertSubroutineName(aUnit,fs.Comments(stmt))
                        self.__myNewExecs.append(newStmt)
                    endline = re.search('[\n]',anExecStmt.rawline[match.end():])
                    if endline:
                        end = match.end()+endline.start()
                        pragma = int(anExecStmt.rawline[match.end():end].strip())
                    else:
                        pragma = int(anExecStmt.rawline[match.end()].strip())
                    if pragma < len(Execs):
                        # return to input
                        self.__appendNonEmptyStmt(Execs[pragma],self.__myNewExecs)
                    # continue template
                    newmatch = num_pat.search(anExecStmt.rawline[match.end():])
                    stmt = anExecStmt.rawline[match.end()+newmatch.end():]
                    if len(stmt.strip()) != 0:
                        newStmt = self.__insertSubroutineName(aUnit,fs.Comments(stmt))
                        self.__myNewExecs.append(newStmt)
                else:
                    anExecStmt = self.__insertSubroutineName(aUnit,anExecStmt)
                    self.__myNewExecs.append(anExecStmt)
                continue
            anExecStmt = self.__insertSubroutineName(aUnit,anExecStmt)
            self.__myNewExecs.append(anExecStmt)


    # gets the name of the template used
    # RETURNS: the name of the template file (default, if none specified)
    def __getTemplateName(self):
        if self.__myUnit.cmnt is not None:
            template = \
                self.__getTemplateFromComment(self.__myUnit.cmnt)
            if template is not None:
                return template
        stmtList=[]
        map(stmtList.extend,[self.__myUnit.decls,self.__myUnit.execs,self.__myUnit.end])
        for aStmt in filter(lambda l: l.is_comment(),stmtList):
            template = self.__getTemplateFromComment(aStmt)
            if template is not None:
                return template
        return TemplateExpansion._templateFile #default template file

    # extracts the template name from a comment
    # PARAMS:
    # comment -- the input comment to be searched for a template name
    # RETURNS: the name specified by the comment or None
    def __getTemplateFromComment(self,comment):
        name = None
        p = re.compile('[$]openad[ ]+xxx[ ]+template[ ]+',re.IGNORECASE)
        match = p.search(comment.rawline)
        if match:
            end_name = re.search('[\n]|[ ]',comment.rawline[match.end():])
            if end_name:
                name = (comment.rawline[match.end():(match.end()+end_name.start())]).strip()
            else:
                name = (comment.rawline[match.end():]).strip()
        return name

    # replace '__SRNAME__' with the name of the subroutine being inlined
    # PARAMS:
    # Unit: the subroutine being processed (the one to replace __SRNAME__ with)
    # anExecStmt: an executive statement in which to replace all instances of
    # '__SRNAME__'
    def __insertSubroutineName(self,Unit,anExecStmt):
        srname=re.compile('__SRNAME__',re.IGNORECASE)
        match = srname.search(anExecStmt.rawline)
        if match:
            if isinstance(anExecStmt,fs.Comments):
                plainLine = anExecStmt.rawline
            else:
                plainLine=str(anExecStmt) # the line w/o continuation+linebreaks

            plMatch=srname.search(plainLine)
            while plMatch:
                plainLine=plainLine[:plMatch.start()]+Unit.uinfo.name+plainLine[plMatch.end():]
                plMatch=srname.search(plainLine)

            if isinstance(anExecStmt,fs.Comments):
                aNewExecStmt = fs.Comments(plainLine)
            else:
                # redo scan and parse for the class of the given anExecStmt
                aNewExecStmt=anExecStmt.__class__.parse(scan1.scan(plainLine)[0],anExecStmt.lineNumber)
                aNewExecStmt.lead=anExecStmt.lead
            return aNewExecStmt
        return anExecStmt

    # Given a template file 'template' and the Decls and Execs from the file
    # being post-processed in reverse mode, insert all appropriate Decls, Execs, 
    # and inlined statements from template, inline, and original files in the unit
    # PARAMS:
    # Decls: a list of lists of processed declaration statements from the input
    # file, indexed by replacement pragma number
    # Execs: a list of lists of processed executive statements from the input
    # file, indexed by replacement pragma number
    # RETURNS: a processed unit after template expansion
    def expandTemplate(self,Decls,Execs):
        template = self.__getTemplateName()
        inputDeclNum = 0
        inputExecNum = 0
        pragma = 0
        aUnit=getTemplateUnit(template)    
        if aUnit.cmnt is not None:
            if self.__myUnit.cmnt is not None:
                self.__myUnit.cmnt.rawline = ''.join([aUnit.cmnt.rawline,self.__myUnit.cmnt.rawline])
            else:
                self.__myUnit.cmnt = aUnit.cmnt
            
        if isinstance(aUnit.uinfo,fs.SubroutineStmt):
            aUnit.uinfo.name = self.__myUnit.uinfo.name
            
        self.__expandTemplateDecls(aUnit, Decls)

        self.__expandTemplateExecs(aUnit, Execs)

        newEndStmts = []
        template=re.compile("template",re.IGNORECASE)
        for endStmt in aUnit.end:
            newEndStmts = []
            if isinstance(endStmt,fs.EndStmt):
                match = template.search(endStmt.rawline)
                if match:
                    newEndStmt=endStmt.__class__(self.__myUnit.uinfo.name,\
                                                 lineNumber=endStmt.lineNumber,\
                                                 label=endStmt.label,lead=endStmt.lead)
                    newEndStmts.append(newEndStmt)
                else: 
                    newEndStmts.append(endStmt)
        self.__myUnit.end = newEndStmts    
        self.__myUnit.decls = self.__myNewDecls
        self.__myUnit.execs = self.__myNewExecs
        return self.__myUnit
