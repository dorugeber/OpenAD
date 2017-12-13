// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <sys/time.h>
#include <cstring>
#include <iomanip>
#include <cerrno>

#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/utils/inc/Thread.hpp"

namespace xaifBooster { 

  DbgLoggerManager* DbgLoggerManager::ourInstance_p=0;
  Mutex DbgLoggerManager::ourInstanceMutex;

  DbgLoggerManager* 
  DbgLoggerManager::instance() { 
    if (ourInstance_p)
      return ourInstance_p;
    ourInstanceMutex.lock();
    try { 
      if (!ourInstance_p)
	ourInstance_p=new DbgLoggerManager();
      if (!ourInstance_p) { 
	THROW_LOGICEXCEPTION_MACRO("DbgLoggerManager::instance");
      } // end if 
    } // end try 
    catch (...) { 
      ourInstanceMutex.unlock();
      throw;
    } // end catch
    ourInstanceMutex.unlock();
    return ourInstance_p;
  } // end of DbgLoggerManager::instance

  void
  DbgLoggerManager::setSelection(unsigned int aSelection) { 
    ourInstanceMutex.lock();
    if (aSelection==0) {
      if(myLogger_p) { 
	delete myLogger_p;
	myLogger_p=NULL; 
      } // end if 
    } // end if
    mySelector=aSelection;
    ourInstanceMutex.unlock();
  } // end of DbgLoggerManager::setSelection

  void
  DbgLoggerManager::setFile(const std::string& aFileName_c) { 
    ourInstanceMutex.lock();
    try { 
      if (aFileName_c!=myDebugOutPutFileName) {  
	if (myLogger_p) { 
	  delete myLogger_p;
	  myLogger_p=NULL; 
	}
	myOutFileStream.close();
	myOutFileStream.clear();
	errno=0;
	myOutFileStream.open(aFileName_c.c_str(), 
			     std::ios::app);
	int keepErrno=errno;
	if(! myOutFileStream || keepErrno) {
	  ourInstanceMutex.unlock();
	  THROW_LOGICEXCEPTION_MACRO("Cannot open " 
				     << aFileName_c.c_str() 
				     << " for server debug. System message: "
				     << strerror(keepErrno));
	} 
	myDebugOutPutFileName=aFileName_c;
      }  // end if 
    } // end try 
    catch (LogicException& e) { 
      throw e;
    }
    catch (...) {
      ourInstanceMutex.unlock();
      THROW_LOGICEXCEPTION_MACRO("Exception in DbgLoggerManager::setFile"); 
    } // end catch 
    ourInstanceMutex.unlock();
  } // end of DbgLoggerManager::setFile

  void 
  DbgLoggerManager::setBinaryBuildInfo(const std::string& aBuildInfo_c) { 
    myBinaryBuildInfo=aBuildInfo_c;
  } 

  void
  DbgLoggerManager::setGraphicsFormat(const std::string& aGraphicsFormatString) {
    if (aGraphicsFormatString == "svg")
      myGraphicsFormat = DbgLoggerManager::SVG_FORMAT;
  } // end DbgLoggerManager::setGraphicsFormat()

  void
  DbgLoggerManager::setSvgViewer(const std::string& aSvgViewerString) {
    mySvgViewerString = aSvgViewerString;
  }

  unsigned int
  DbgLoggerManager::getSelection() const {
    return(mySelector);
  }

  std::string
  DbgLoggerManager::getFileName() const {
    return(myDebugOutPutFileName);
  }

  DbgLoggerManager::DBG_Graphics_Format_E
  DbgLoggerManager::getGraphicsFormat() const {
    return myGraphicsFormat;
  } // end DbgLoggerManager::getGraphicsFormat()

  std::string
  DbgLoggerManager::getSvgViewer() const {
    return mySvgViewerString;
  }

  DbgLoggerManager::DbgLoggerManager() : 
    myLogger_p(NULL), 
    myDebugOutPutFileName(""),
    myGraphicsFormat(DbgLoggerManager::PS_FORMAT),
    mySvgViewerString("firefox"),
    mySelector(0),
    myPreviousS(0),
    myPreviousMS(0) { 
  } // end of DbgLoggerManager::DbgLoggerManager
  
  DbgLoggerManager::~DbgLoggerManager() { 
    if (myLogger_p)
      delete myLogger_p;
    if (!(myOutFileStream.fail()))
      myOutFileStream.close();
  } // end of DbgLoggerManager::~DbgLoggerManager

  bool 
  DbgLoggerManager::isSelected(const DbgGroup::DbgGroup_E aGroup_c) const { 
    if ((aGroup_c & mySelector) || aGroup_c == DbgGroup::ERROR)
      return (true);
    else
      return (false);

  } // end of DbgLoggerManager::isSelected

  void  
  DbgLoggerManager::addTags(const std::string& spaceSeparatedTags) { 
    std::string::size_type startPosition=0,endPosition=1; // end pos. should at least be 1 or we have an empty string
    std::string::size_type totalSize(spaceSeparatedTags.size());
    while (startPosition<=totalSize && endPosition<=totalSize) { 
      startPosition=spaceSeparatedTags.find_first_not_of(' ',startPosition);
      endPosition=spaceSeparatedTags.find_first_of(' ',startPosition);
      myTagSet.insert(spaceSeparatedTags.substr(startPosition,
						endPosition-startPosition));
      startPosition=endPosition;
    } 
  } 

  bool DbgLoggerManager::wantTag(const std::string& aTag) const { 
    if (myTagSet.empty()) 
      return true;
    return (myTagSet.find(aTag)!=myTagSet.end());
  } 

  void 
  DbgLoggerManager::logDebug(const std::string aFileName_c,
			     const int aLineNumber_c,
			     const DbgGroup::DbgGroup_E aGroup_c, 
			     std::ostringstream& aMessage_r,
			     const std::string& aTag) { 
    if (isSelected(aGroup_c)) { 
      std::ostringstream message; 
      message << Thread::threadId()  
	      << ":" 
	      << aFileName_c.c_str()
	      << ":"
	      << aLineNumber_c
	      << ":"
	      << DbgGroup::toString(aGroup_c)
	      << ":"
	      << aTag.c_str()
	      << ":"; 
      if (aGroup_c & DbgGroup::TIMING) {
	timeval aTimeVal;
	gettimeofday(&aTimeVal,0);
	message << std::setw(10)
		<< aTimeVal.tv_sec
		<< "."
		<< std::setw(6)
		<< aTimeVal.tv_usec
		<< ":"
		<< std::setw(10)
		<< ((aTimeVal.tv_usec<myPreviousMS)?aTimeVal.tv_sec-myPreviousS-1:aTimeVal.tv_sec-myPreviousS)
		<< "."
		<< std::setw(6)
		<< ((aTimeVal.tv_usec<myPreviousMS)?1000000-(myPreviousMS-aTimeVal.tv_usec):aTimeVal.tv_usec-myPreviousMS)
		<< ":"; 
	myPreviousS=aTimeVal.tv_sec;
	myPreviousMS=aTimeVal.tv_usec;
      } 
      message << aMessage_r.str().c_str();
      if (myLogger_p) { 
	myLogger_p->logMessage(message);
	return; 
      } 
      ourInstanceMutex.lock();
      try { 
	if (!myLogger_p) { 
	  if (myDebugOutPutFileName.empty())
	    myLogger_p=new DbgLogger(std::cout);
	  else {
	    myOutFileStream.close();
	    myOutFileStream.open(myDebugOutPutFileName.c_str(),
				 std::ios::app);
	    if(!myOutFileStream) {
	      std::cerr << "DbgLoggerManager::logDebug unable to open " 
			<< myDebugOutPutFileName 
			<< std::endl;
	      ourInstanceMutex.unlock();
	      return; 
	    } 
	    myLogger_p=new DbgLogger(myOutFileStream);
	  } // end else 
	  if (!myLogger_p) 
	    std::cerr << "DbgLoggerManager::logDebug: out of memory"
		      << std::endl;
	  else { 
	    // startup message
	    std::ostringstream startupMessage;
	    if (!myBinaryBuildInfo.empty())
	      startupMessage << "=== " << myBinaryBuildInfo.c_str() << " === reset log";
	    else
	      startupMessage << "========================================== reset log"; 
	    myLogger_p->logMessage(startupMessage);
	  } // end else 
	} // end if 
      } // end try 
      catch (...) {
	std::cerr << "DbgLoggerManager::logDebug: unexpected exception"
		  << std::endl; 
      } // end catch
      ourInstanceMutex.unlock();
      if (myLogger_p) 
	myLogger_p->logMessage(message);
    } // end if  
  } // end of DbgLoggerManager::logDebug
    
}


 
