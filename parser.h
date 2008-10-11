/*
 parser.h - demonstration of a parser in C++
*/

#ifndef PARSER_H
#define PARSER_H

#include <qstring.h>
#include <QtXml/QXmlDefaultHandler>
#include <QtXml/QXmlAttributes>

class Parsert : public QXmlDefaultHandler
{
public:
  Parsert();

  /** given by the framework from qxml. Called when parsing the xml-document starts.          */
  bool startDocument();
 
  void setDocumentLocator(QXmlLocator*);
  int lineNumber();

  /** given by the framework from qxml. Called when the reader occurs an open tag (e.g. \<b\> ) */
  bool startElement( const QString&, const QString&, const QString& qName, const QXmlAttributes& att );
  bool endElement( const QString&, const QString&, const QString& qName );
  bool characters(const QString& ch);
  QString heading(int i);
  QXmlLocator* loc;
  bool withInHeading;
  int bodystartx; // where the html body starts
  int bodystarty; // where the html body starts
  int getbodystarty();
  int getbodystartx();
  int headingcount();
  QList<QString> headings;
};


#endif
