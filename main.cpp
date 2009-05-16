/*
add_toc (c) 2006-2008 by Thorsten Staerk
*/
#include <iostream>
#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <kdebug.h>
#include <KMessageBox>
#include <KUrl>
#include <QFile> 
#include <QDomDocument>
#include <QTextEdit>
#include <tidy/tidy.h>
#include <tidy/buffio.h>
#include <stdio.h>
#include <errno.h>

QString tidy(char* input)
// take html code and return it converted to xhtml code
{                                                        
  kDebug() << "Entering function";                       
  // the following code is (c) Charles Reitzel and Dave Raggett, see the package tidy                                            
  TidyBuffer output = {0};                                                         
  TidyBuffer errbuf = {0};   
  QString result;                                                      
  int rc = -1;                                                                     
  Bool ok;                                                                         

  TidyDoc tdoc = tidyCreate();                             // Initialize "document"
  kDebug() << "Tidying:\t\%s\\n" << input;               

  ok = tidyOptSetBool( tdoc, TidyXhtmlOut, yes );          // Convert to XHTML
  if ( ok ) rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
  if ( rc >= 0 ) rc = tidyParseString( tdoc, input );      // Parse the input    
  if ( rc >= 0 ) rc = tidyCleanAndRepair( tdoc );          // Tidy it up!        
  if ( rc >= 0 ) rc = tidyRunDiagnostics( tdoc );          // Kvetch             
  if ( rc > 1 )                                            // If error, force output.
    rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );                   
  if ( rc >= 0 ) rc = tidySaveBuffer( tdoc, &output );     // Pretty Print           
  if ( rc >= 0 )                                                                     
  {                                                                                  
    if ( rc > 0 ) kDebug() << "\\nDiagnostics:\\n\\n\%s" << errbuf.bp;
    char* outputstring; // content of the outputfile                                 

    // find out length of outputstring
    int length=0; // length of outputstring
    byte* string=output.bp;                
    while (*string)                        
    {                                      
      string++;                                                
      length++;                                                
    }

    kDebug() << "allocating memory " << length;
    outputstring=(char*)malloc(length);
    snprintf(outputstring,length,"%s",output.bp);
    result=QString::fromLocal8Bit(outputstring,length);
  }
  else
    printf( "A severe error (\%d) occurred.\\n", rc );
  tidyBufFree( &output );
  tidyBufFree( &errbuf );
  tidyRelease( tdoc );
  return result;
}

QString output(QDomNode node, bool isheading)
{
  QDomNode next;
  QString result;
  kDebug() << node.nodeName();
  if (node.nodeName()=="h1") isheading=true;
  if (node.nodeName()=="h2") isheading=true;
  if (node.nodeName()=="h3") isheading=true;
  if (node.nodeName()=="h4") isheading=true;
  if (node.nodeName()=="h5") isheading=true;
  if (node.nodeName()=="h6") isheading=true;
  if (node.isText() && isheading) result.append(QString(node.nodeValue())).append(QString("\n"));
  if (node.hasChildNodes()) 
  {
    for (int i=0; i<=node.childNodes().count(); ++i) result.append(output(node.childNodes().at(i), isheading));
  }
  kDebug() << "returning " << result;
  return result;
}

QDomNode firsttextnode(QDomNode node, int level)
// deliver node if node is a text node, or deliver a subnode that is a text node.
{
  QDomNode next;
  kDebug() << node.nodeName() << node.isText() << node.nodeValue() << level;
  if (node.isText() && level>0) return node;
  if (node.hasChildNodes()) 
  {
    for (int i=0; i<=node.childNodes().count(); ++i) 
      if (firsttextnode(node.childNodes().at(i),++level)!=QDomNode()) return firsttextnode(node.childNodes().at(i),level);
  }
  return QDomNode();
}

int main (int argc, char *argv[])
{
  KAboutData aboutData( "addtoc",
			0,
			ki18n("addtoc"),
			"0.1",
			ki18n("Adds a Table Of Content to an html file"),
			KAboutData::License_GPL,
			ki18n("(c) 2006-2008 by Thorsten Staerk"),
			ki18n("This is addtoc"),
			"http://www.staerk.de/thorsten",
			"bugs@staerk.de");
  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineOptions options; 
  options.add("+[file]", ki18n("Document to open")); 
  KCmdLineArgs::addCmdLineOptions(options); 
  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  QByteArray inputfilecontent;
  QString tidycontent;
  KApplication app;
  if (args->count()) 
  {
    kDebug() << args->url(0).url();
    QFile inputfile(args->url(0).fileName());
    inputfile.open(QIODevice::ReadOnly);
    inputfilecontent = inputfile.read(inputfile.bytesAvailable());
    kDebug() << "original file content=" << inputfilecontent;
    tidycontent=tidy(inputfilecontent.data());
    QDomDocument mydom=QDomDocument();
    mydom.setContent(tidycontent);
    QDomNode bodynode(mydom.elementsByTagName("body").at(0));
    output(bodynode,false);
    QDomNode toc;
    toc=bodynode.firstChild().cloneNode(false);
    bodynode.insertBefore(firsttextnode(bodynode,0).cloneNode(false),bodynode.firstChild());
    firsttextnode(bodynode,0).setNodeValue(QString("Table of Content:\n").append(output(bodynode,false)));
    std::cout << mydom.toByteArray().data() << std::endl;
  } 
  else std::cout << "addtoc usage: addtoc <htmlfile>" << std::endl;
}

