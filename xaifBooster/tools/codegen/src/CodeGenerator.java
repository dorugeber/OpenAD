import java.io.*;
import java.util.Map;
import java.util.HashMap;
import java.lang.Integer;

// import sax.helpers.AttributesImpl;

import org.xml.sax.Attributes;
import org.xml.sax.Parser;
import org.xml.sax.SAXException;
import org.xml.sax.SAXNotRecognizedException;
import org.xml.sax.SAXNotSupportedException;
import org.xml.sax.SAXParseException;
import org.xml.sax.XMLReader;
import org.xml.sax.ext.LexicalHandler;
import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.helpers.ParserAdapter;
import org.xml.sax.helpers.ParserFactory;
import org.xml.sax.helpers.XMLReaderFactory;

public class CodeGenerator  
    extends DefaultHandler
    implements LexicalHandler {

    protected static final String NAMESPACES_FEATURE_ID = "http://xml.org/sax/features/namespaces";
    protected static final String VALIDATION_FEATURE_ID = "http://xml.org/sax/features/validation";
    protected static final String SCHEMA_VALIDATION_FEATURE_ID = "http://apache.org/xml/features/validation/schema";
    protected static final String SCHEMA_FULL_CHECKING_FEATURE_ID = "http://apache.org/xml/features/validation/schema-full-checking";
    protected static final String LEXICAL_HANDLER_PROPERTY_ID = "http://xml.org/sax/properties/lexical-handler";
    protected static final String DEFAULT_PARSER_NAME = "org.apache.xerces.parsers.SAXParser";
    protected PrintWriter fOut;

// UN: my variables

// Document

   private static int nr_templates;
   private static int nr_doc_entries;
   private static int[] nr_versions_per_template;
   private static int[] nr_entries_per_template;

   class DocumentElementType {
     boolean is_repeatable;
     NonRepeatableCodeOrLineBreakType myNonRepeatableCodeOrLineBreak;
     RepeatableCodeType myRepeatableCode;
     DocumentElementType() {}
   }

   static DocumentElementType[] myDocument;

// NonRepeatableCodeOrLineBreak

   class NonRepeatableCodeOrLineBreakType {
     boolean is_linebreak;
     String myText;
     NonRepeatableCodeOrLineBreakType() {
       is_linebreak=false;
       myText="";
     }
   }

// RepeatableCode

   class RepeatableCodeType {
     TemplateEntryType[] myTemplateEntry;
     int use_versions;
     HashMap[] myCodeVersion;
     RepeatableCodeType() {
       use_versions=-1;
     }
   }

  // TemplateEntry

   class TemplateEntryType {
     int type; // -1=undef;0=linebreak;1=StaticCode;2=DynamicHook
     String myStaticCode;
     String myDynamicHook;
     TemplateEntryType() {
       type=-1;
       myStaticCode="";
       myDynamicHook="";
     }
   }

  // File I/O
    File outfile;
    FileWriter out;

  // Counters
    private static int doc_entries_counter;
    private static int template_entries_counter;
    private static int template_counter;
    private static int code_version_counter;
    private static int current_code_version;

        
    /** Default constructor. */
    public CodeGenerator() {
    } // <init>()

    public void setOutput(OutputStream stream, String encoding)
        throws UnsupportedEncodingException {

        if (encoding == null) {
            encoding = "UTF8";
        }

        java.io.Writer writer = new OutputStreamWriter(stream, encoding);
        fOut = new PrintWriter(writer);

    } // setOutput(OutputStream,String)

    public void setOutput(java.io.Writer writer) {

        fOut = writer instanceof PrintWriter
             ? (PrintWriter)writer : new PrintWriter(writer);

    } // setOutput(java.io.Writer)

    public void startDocument() throws SAXException {
    } // startDocument()

    public void endDocument() throws SAXException {
      try {
          template_counter=0;
          for (int k = 0; k < nr_doc_entries; k++) { // loop over DocumentEntries
          if (myDocument[k].is_repeatable) {
            int temp = myDocument[k].myRepeatableCode.use_versions;
            for (int i = 0; i < nr_versions_per_template[template_counter]; i++) { // loop over CodeVersions
              for (int j = 0; j < nr_entries_per_template[template_counter]; j++) { // loop over TemplateEntries
                if (myDocument[k].myRepeatableCode.myTemplateEntry[j].type==0) {
                  out.write(10);
                }
                else if (myDocument[k].myRepeatableCode.myTemplateEntry[j].type==1) {
                  out.write(myDocument[k].myRepeatableCode.myTemplateEntry[j].myStaticCode);
                }
                else if (myDocument[k].myRepeatableCode.myTemplateEntry[j].type==2) {
                  if (temp==-1) {
                    out.write((String) myDocument[k].myRepeatableCode.myCodeVersion[i].get(myDocument[k].myRepeatableCode.myTemplateEntry[j].myDynamicHook));
                  }
                  else {
                    out.write((String) myDocument[temp].myRepeatableCode.myCodeVersion[i].get(myDocument[k].myRepeatableCode.myTemplateEntry[j].myDynamicHook));
                  }
                }
              }
            }
            template_counter++;
          }
          else {
            if (myDocument[k].myNonRepeatableCodeOrLineBreak.is_linebreak) {
              out.write(10);
            }
            else {
              out.write(myDocument[k].myNonRepeatableCodeOrLineBreak.myText);
            }
          }
        }
        out.close();
      }
      catch (Exception e) { 
        System.err.println("Exception caught"); 
        e.printStackTrace(System.err);
      }
    } // endDocument()

    public void processingInstruction(String target, String data) {
    } // processingInstruction(String,String)

    public void startElement(String uri, String local, String raw,
                             Attributes attrs) throws SAXException {
        if (local=="Document") {
          Integer temp = new  Integer(attrs.getValue(1));
          nr_doc_entries=temp.intValue();
          temp = new  Integer(attrs.getValue(2));
          nr_templates=temp.intValue();
          myDocument = new DocumentElementType[nr_doc_entries];
          for (int i=0;i<nr_doc_entries;i++) 
            myDocument[i]=new DocumentElementType();
          nr_versions_per_template = new int[nr_templates];
          nr_entries_per_template = new int[nr_templates];
          doc_entries_counter=0;
          outfile=new File(attrs.getValue(3));
          try {
            out=new FileWriter(outfile);
          }
          catch (Exception e) { 
            System.err.println("Exception caught"); 
            e.printStackTrace(System.err);
          }
        }
        if (local=="NonRepeatableCodeOrLineBreak") {
          myDocument[doc_entries_counter].myNonRepeatableCodeOrLineBreak= new NonRepeatableCodeOrLineBreakType();
            myDocument[doc_entries_counter].is_repeatable=false;
          if (attrs.getLength()==0) {
            myDocument[doc_entries_counter++].myNonRepeatableCodeOrLineBreak.is_linebreak=true;
          }
          else {
            myDocument[doc_entries_counter++].myNonRepeatableCodeOrLineBreak.myText=attrs.getValue(0);
          }
        }
        else if (local=="RepeatableCode") {
            myDocument[doc_entries_counter].myRepeatableCode= new RepeatableCodeType();
            myDocument[doc_entries_counter].is_repeatable=true;
            code_version_counter=0;

/*
 * The number of versions per template is stored as an attribute
 * in RepeatableCode
 */
            Integer temp = new  Integer(attrs.getValue(1));
            myDocument[doc_entries_counter].myRepeatableCode.use_versions=temp.intValue();
            current_code_version=temp.intValue();
            temp = new  Integer(attrs.getValue(0));
            nr_versions_per_template[template_counter]=temp.intValue();
            if (myDocument[doc_entries_counter].myRepeatableCode.use_versions==-1) {
              current_code_version=doc_entries_counter;
              myDocument[doc_entries_counter].myRepeatableCode.myCodeVersion=new HashMap[nr_versions_per_template[template_counter]];
              for (int i=0;i<nr_versions_per_template[template_counter];i++) 
                myDocument[doc_entries_counter].myRepeatableCode.myCodeVersion[i]=new HashMap();
            }
        }
        else if (local=="CodeTemplate") {
            template_entries_counter=0;
            Integer temp = new  Integer(attrs.getValue(0));
            nr_entries_per_template[template_counter]=temp.intValue();
            myDocument[doc_entries_counter].myRepeatableCode.myTemplateEntry=new TemplateEntryType[nr_entries_per_template[template_counter]];
            for (int i=0;i<nr_entries_per_template[template_counter];i++) 
              myDocument[doc_entries_counter].myRepeatableCode.myTemplateEntry[i]=new TemplateEntryType();
        }
        else if (local=="LineBreak") {
            myDocument[doc_entries_counter].myRepeatableCode
                    .myTemplateEntry[template_entries_counter++].type=0;
        }
        else if (local=="StaticCode") {
            myDocument[doc_entries_counter].myRepeatableCode
                    .myTemplateEntry[template_entries_counter].type=1;
            myDocument[doc_entries_counter].myRepeatableCode
                    .myTemplateEntry[template_entries_counter++].myStaticCode=attrs.getValue(0);
        }
        else if (local=="DynamicCodePlaceholder") {
            myDocument[doc_entries_counter].myRepeatableCode
                    .myTemplateEntry[template_entries_counter].type=2;
            myDocument[doc_entries_counter].myRepeatableCode
                    .myTemplateEntry[template_entries_counter++].myDynamicHook=attrs.getValue(0);
        }
        else if (local=="CodeVersion") {
        }
        else if (local=="DynamicCode") {
            myDocument[current_code_version].myRepeatableCode.myCodeVersion[code_version_counter].put(attrs.getValue(0),attrs.getValue(1));
        }
    } // startElement(String,String,String,Attributes)

    public void endElement(String uri, String local, String raw) throws SAXException {
        if (local=="RepeatableCode") {
            doc_entries_counter++;
            template_counter++;
        }
        else if (local=="CodeVersion") {
            code_version_counter++;
        }
    } // endElement(String,String,String)

    /** Characters. */
    public void characters(char ch[], int start, int length)
        throws SAXException {
    } // characters(char[],int,int);

    /** Ignorable whitespace. */
    public void ignorableWhitespace(char ch[], int start, int length)
        throws SAXException {
    } // ignorableWhitespace(char[],int,int);

    public void warning(SAXParseException ex) throws SAXException {
        printError("Warning", ex);
    } // warning(SAXParseException)

    /** Error. */
    public void error(SAXParseException ex) throws SAXException {
        printError("Error", ex);
    } // error(SAXParseException)

    /** Fatal error. */
    public void fatalError(SAXParseException ex) throws SAXException {
        printError("Fatal Error", ex);
        throw ex;
    } // fatalError(SAXParseException)

    //
    // LexicalHandler methods
    //

    /** Start DTD. */
    public void startDTD(String name, String publicId, String systemId)
        throws SAXException {
    } // startDTD(String,String,String)

    /** End DTD. */
    public void endDTD() throws SAXException {
    } // endDTD()

    /** Start entity. */
    public void startEntity(String name) throws SAXException {
    } // startEntity(String)

    /** End entity. */
    public void endEntity(String name) throws SAXException {
    } // endEntity(String)

    /** Start CDATA section. */
    public void startCDATA() throws SAXException {
    } // startCDATA()

    /** End CDATA section. */
    public void endCDATA() throws SAXException {
    } // endCDATA()

    /** Comment. */
    public void comment(char ch[], int start, int length) throws SAXException {
    } // comment(char[],int,int)

    /** Prints the error message. */
    protected void printError(String type, SAXParseException ex) {
        System.err.print(type);
        ex.printStackTrace(System.err);
       
    } // printError(String,SAXParseException)

    public static void main(String argv[]) {

        // variables
        CodeGenerator writer = null;
        XMLReader parser = null;
        boolean namespaces = true;
        boolean validation = true;
        boolean schemaValidation = true;
        boolean schemaFullChecking = true;
        boolean canonical = false;

            // create parser
            try {
               parser = XMLReaderFactory.createXMLReader(DEFAULT_PARSER_NAME);
            }
            catch (Exception e) {
               System.err.println("error: Unable to instantiate parser ("+DEFAULT_PARSER_NAME+")");
               e.printStackTrace(System.err);
            }

            // set parser features
            try {
                parser.setFeature(NAMESPACES_FEATURE_ID, namespaces);
            }
            catch (SAXException e) {
                System.err.println("warning: Parser does not support feature ("+NAMESPACES_FEATURE_ID+")");
            }
            try {
                parser.setFeature(VALIDATION_FEATURE_ID, validation);
            }
            catch (SAXException e) {
                System.err.println("warning: Parser does not support feature ("+VALIDATION_FEATURE_ID+")");
            }
            try {
                parser.setFeature(SCHEMA_VALIDATION_FEATURE_ID, schemaValidation);
            }
            catch (SAXNotRecognizedException e) {
                // ignore
            }
            catch (SAXNotSupportedException e) {
                System.err.println("warning: Parser does not support feature ("+SCHEMA_VALIDATION_FEATURE_ID+")");
            }
            try {
                parser.setFeature(SCHEMA_FULL_CHECKING_FEATURE_ID, schemaFullChecking);
            }
            catch (SAXNotRecognizedException e) {
                // ignore
            }
            catch (SAXNotSupportedException e) {
                System.err.println("warning: Parser does not support feature ("+SCHEMA_FULL_CHECKING_FEATURE_ID+")");
            }

            // setup writer
            if (writer == null) {
                writer = new CodeGenerator();
                try {
                    writer.setOutput(System.out, "UTF8");
                }
                catch (UnsupportedEncodingException e) {
                    System.err.println("error: Unable to set output. Exiting.");
                    System.exit(1);
                }

            // set parser
            parser.setContentHandler(writer);
            parser.setErrorHandler(writer);
            try {
                parser.setProperty(LEXICAL_HANDLER_PROPERTY_ID, writer);
            }
            catch (SAXException e) {
                // ignore
            }

            // parse file
            try {
                parser.parse(argv[0]);
            }
            catch (SAXParseException e) {
                // ignore
            }
            catch (Exception e) {
                System.err.println("error: Parse error occurred - "+e.getMessage());
                if (e instanceof SAXException) {
                    e = ((SAXException)e).getException();
                }
                e.printStackTrace(System.err);
            }
        }

    } // main(String[])

} // class CodeGenerator
