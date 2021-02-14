#include <QCoreApplication>
#include "song.h"
#include <iostream>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    song s;
    s.deserialize();

    cout << s.getRating()<< endl;

    //izmeni rating na 99
    cout << "------------------------------------" << endl;
    s.setRating(12345);
    cout << s.getRating()<< endl;
    s.saveToFile();

//    return a.exec();
}
