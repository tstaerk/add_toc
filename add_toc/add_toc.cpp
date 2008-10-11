/*
add_toc.cpp

This program adds a table of content (toc) at the beginning of an html file.

compile it e.g. with
g=$(echo -en "g\0053\0053")
$g -I. -I/home/kde-devel/kde/include -I/home/kde-devel/qt-unstable/include/Qt -I/home/kde-devel/qt-unstable/include -I/home/kde-devel/qt-unstable/include/QtXml parser.h parser.cpp add_toc.cpp -o add_toc -L/home/kde-devel/kde/lib -L/home/kde-devel/qt-unstable/lib -lQtCore_debug -lQtXml_debug -lkdeui
*/

#include <iostream>
#include <KAboutData>
#include <KApplication>
#include <KCmdLineArgs>
#include <qstring.h>
#include <QXmlInputSource>
#include <QXmlLocator>
#include <qfile.h>
#include <parser.h>
#include <kdebug.h>

using namespace std;

int main( int argc, char *argv[] )
{  
  KAboutData aboutData( "add_toc", 0, ki18n("add_toc"),
      "0.1", ki18n("add a toc to an html file"), KAboutData::License_GPL,
      ki18n("(c) 2006-2008, Thorsten Staerk") );

  aboutData.addAuthor( ki18n("Thorsten Staerk"), ki18n( "Current Maintainer" ),
                       "me@home.de" );

  KCmdLineArgs::init( argc, argv, &aboutData );

  KCmdLineOptions options;
  options.add("+file", ki18n( "This is an argument" ));
  KCmdLineArgs::addCmdLineOptions( options );
  KApplication myApp;

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  Parsert* handler=new Parsert();
  QFile* htmlfile=new QFile("hello.htm");
  if ( args->count() > 0 )
  {
    kDebug() << args->arg( 0 );
    htmlfile=new QFile(args->arg(0));
    QXmlInputSource* source=new QXmlInputSource(htmlfile);
    QXmlSimpleReader reader;
  
    reader.setContentHandler( handler );
    reader.parse( source );
    htmlfile->close();
    htmlfile->open(QIODevice::ReadOnly);
    QFile* newfile=new QFile("new.html");
    newfile->open(QIODevice::WriteOnly);
    for (int i=0; i<handler->getbodystarty(); i++)
    {
      QByteArray qb=htmlfile->readLine();
      newfile->write(qb);
    }

    kDebug() << "headingcount is " << handler->headingcount() << endl;
    QByteArray qb; 
    int n=handler->headingcount();
    for (int i=0; i<n; i++)
    {
      kDebug() << "i is " << i << endl;
      kDebug() << "heading count is " << handler->headingcount() << endl;
      qb=QByteArray(handler->heading(0).toAscii());
      newfile->write(qb);
      newfile->write("<br />\n");
    }
    qb=htmlfile->readLine();
    while (qb!=QByteArray()) 
    {
      newfile->write(qb);
      qb=htmlfile->readLine();
    }
    newfile->close();
    htmlfile->close();
  }
  else
  {
    cout << "Please name a file to add a toc to, like this:\n" << endl;
    cout << "add_toc example.htm" << endl;
  }
}
