import PyFort.fortExp as fe
import re

# Replaces inline args with the given replacement args
# PARAMS:
# number of arguments to look at); equal to the minimum of the number of
# inlineArgs and number of replacementArgs
# string -- the string in which arguments must be replaced
# inlineArgs -- arguments from the inline file (args to be replaced)
# replacementArgs -- arguments from the input file being processed
# RETURNS: a modified strings with all inlineArgs replaced by the 
# appropriate argument from replacementArgs
def replaceArgs(string,inlineArgs,replacementArgs):
    replDict=dict(zip(inlineArgs,map(str,replacementArgs)))
    rExp=re.compile('|'.join(map(lambda l:'(\\b'+l+'\\b)',map(re.escape,replDict))))
    def oneReplaceMatch(match):
        return replDict[match.group(0)]
    nStr=re.sub(rExp,oneReplaceMatch, string)    
    return nStr

# Replaces inline args with the given args
# called on _son attributes
# PARAMS:
# number of arguments to look at); equal to the minimum of the number of
# inlineArgs and number of replacementArgs
# arg -- the expression to be modified (is one of the sons of a statement)
# inlineArgs -- arguments from the inline file (args to be replaced)
# replacementArgs -- arguments from the input file being processed
# RETURNS: a modified expression to replace the old son in the statement
# being processed
def replaceSon(arg,inlineArgs,replacementArgs):
    index=inlineArgs.index
    newSon = arg
    if isinstance(arg,fe.Sel):
        if arg.head in inlineArgs and len(replacementArgs)>index(arg.head):
            head=replacementArgs[index(arg.head)]
            newSon=fe.Sel(head,arg.proj)
    elif isinstance(arg,fe.App):
        head = arg.head
        args = arg.args
        newArgs = []
        newArgsAppend=newArgs.append
        for anArg in args:
            if isinstance(anArg,fe.App) or isinstance(anArg,fe.Sel):
                newArgsAppend(replaceSon(anArg,inlineArgs,replacementArgs))
            else:
                if anArg in inlineArgs and len(replacementArgs)>index(anArg):
                    newArg=replacementArgs[index(anArg)]
                    newArgsAppend(newArg)
                else:
                    newArgsAppend(anArg)
        if len(newArgs) != 0:
            args = newArgs
        if head in inlineArgs and len(replacementArgs)>index(head):
            head=replacementArgs[index(head)]
        newSon = fe.App(head,newArgs)
    elif isinstance(arg,fe.Ops):
        newSon=fe.Ops(arg.op,
                      replaceArgs(str(arg.a1),inlineArgs,replacementArgs),
                      replaceArgs(str(arg.a2),inlineArgs,replacementArgs))
    elif arg in inlineArgs and len(replacementArgs)>index(arg):
            newSon=replacementArgs[index(arg)]
    return newSon
