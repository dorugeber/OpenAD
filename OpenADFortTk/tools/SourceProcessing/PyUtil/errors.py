'''
specific exceptions that we may want to catch 
at the top level rather than the system exceptions
which should continue to produce a stack trace.
'''

class UserError(Exception):
    '''exception for errors caused by the user'''
    def __init__(self,msg):
        self.msg  = msg

    def __str__(self):
        errString='\nERROR: UserError :'+str(self.msg)
        return (errString)

class ScanError(Exception):
    '''
	exception for errors detected in the scanner caused either by 
	faulty code or by incomplete logic in the scanner implementation
    '''
    def __init__(self,lineNumber,aFortLine,scanned,rest):
        self.lineNumber = lineNumber
        self.aFortLine=aFortLine
	self.scanned=scanned
	self.rest=rest

    def __str__(self):
        errString='\nERROR: ScanError at line '+str(self.lineNumber)+':\n'
        errString+=str(self.aFortLine)+'\n'
        errString+=(len(str(self.aFortLine))-len(str(self.rest)))*' '+'^'
        errString+="\nTokens scanned ok: "+str(self.scanned)+'\tUnable to scan: "'+str(self.rest)+'"'
        if (self.rest == '&' and (config.inputFormat=='fixed')):
            errString+="\nThis failure is likely due to running this script on free-formatted code without specifying the --inputFormat=free flag."
        else:
            errString+="\nThis failure is likely due to possibly legal but unconventional Fortran,"
            errString+="such as unusual spacing. Please consider modifying your source code."
        return str(errString)

class ParseError(Exception):
    '''
	exception for errors detected in the parser caused either by 
	faulty code or by incomplete logic in the parser implementation
    '''
    def __init__(self,lineNumber,scannedLine,target,details=None):
        '''
        the parser failed to parse scannedLine as a target
        where target is some string to indicated verbally what it is
        '''
        self.lineNumber = lineNumber
        self.scannedLine=scannedLine
        self.target=target
        self.details=details

    def __str__(self):
        errString='\nERROR: ParseError: parser fails to assemble tokens in scanned line '+str(self.lineNumber)+':'
        errString+=str(self.scannedLine)
        if self.details: errString+=str(self.details)
        if self.target: errString+="tried to parse as"+str(self.target)
        return (errString)

class LogicError(Exception):
    '''
	exception for general logic errors
    '''
    def __init__(self,reason,lineNumber=None):
        '''
        the parser failed to parse scannedLine as a target
        where target is some string to indicated verbally what it is
        '''
        self.lineNumber = lineNumber
        self.reason=reason

    def __str__(self):
        errString='\nERROR: LogicError: '+str(self.reason)
        if self.lineNumber: errString+='line '+str(self.lineNumber)
        return (errString)
