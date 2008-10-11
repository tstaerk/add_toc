/*
 parser.cpp - demonstration of a parser in C++
*/

#include "parser.h"
#include <kdebug.h>

  Parsert::Parsert()
  {
  }

  int Parsert::lineNumber()
  {
  }
  

  void Parsert::setDocumentLocator(QXmlLocator* l)
  {
    loc=l;
  }

  bool Parsert::startDocument()
  {
    return true;
  }
  
  bool Parsert::startElement( const QString&, const QString&, const QString& qName, const QXmlAttributes& att )
  {
    QRegExp regex("[h|H][1-9][0-9]*");
    if (regex.exactMatch(qName))
    {
      withInHeading=true;
    }
    else withInHeading=false;
    if (qName==QString("body"))
    {
      bodystarty=loc->lineNumber();
      bodystartx=loc->columnNumber();
    }
    return true;
 }

  bool Parsert::endElement( const QString&, const QString&, const QString& qName )
  {
    withInHeading=false;
    return true;
  }

  bool Parsert::characters( const QString& ch)
  {
    if (withInHeading) 
    {
      kDebug() << "heading is named" << ch << endl;
      headings.append(ch);
    }

    return true;
  }
 
  int Parsert::getbodystarty() { return bodystarty; }

  int Parsert::getbodystartx() { return bodystartx; }

  int Parsert::headingcount() { return headings.size(); }

  QString Parsert::heading(int i)
  {
    return headings.takeAt(i);
  }
