#include "song.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <iostream>

using namespace std;

void song::load(QVariant data)
{
    QVariantMap map = data.toMap();
    m_album  = map.value("Album").toString();
    m_artist = map.value("Artist").toString();
    m_rating = map.value("Rating").toDouble();
    m_title  = map.value("Title").toString();
    m_year   = map.value("Year").toUInt();

    QVariantList feature = map.value("Features").toList();
    if(!feature.empty())
    {
        //spreci dva puta punjenje vektora
        m_features.clear();
        m_features.resize(0);
        for(auto f : feature){
            m_features.append(f.toString());
        }
    }


    QVariantList genres = map.value("Genres").toList();
    if(!genres.empty())
    {
        //spreci dva puta punjenje vektora
        m_genres.clear();
        m_genres.resize(0);
        for(auto f : genres)
            m_genres.append(f.toString());
    }

}

QVariant song::save() const{

    QVariantMap map;
    map.insert("Artist",m_artist);
    map.insert("Title",m_title);
    map.insert("Album",m_album);
    QVariantList features;
    for(const auto &f : m_features)
    {
        features.append(f);
    }
    map.insert("Features",features);
    map.insert("Rating",m_rating);

    QVariantList genres;
    for(const auto &g : m_genres)
    {
        genres.append(g);
    }
    map.insert("Genres",genres);
    map.insert("Year",m_year);
    return map;
}

void song::deserialize()
{
    QFile file{"pesme.xml"};
    if(!file.open(QFile::ReadOnly))
    {
        cout << "Neuspelo citanje iz fajla!" << endl;
    }
    QXmlStreamReader reader{&file};
    QVariantMap map;
    reader.readNextStartElement(); //<Encoding>
    //citanje iz xml-a
    while(reader.readNextStartElement())//1 song po 1 song
    {
        auto name = reader.name().toString();
        QString value;
        //analiza sta sam procitao unutar songa
        if( name == "Artist" || name == "Title" || name == "Album")
        {   value = reader.readElementText();
            map.insert(name,value);
        }

        if(name == "Features")
        {
            //preskoci Feature etiketu
            QVariantList xs;
            //moram 1 po 1 element da citam iz liste ...
            while (reader.readNextStartElement()) {
                value = reader.readElementText();
                xs.append(value);
            }
           map.insert("Features",xs);
        }

        if( name == "Rating")
        {
            value =reader.readElementText();
            map.insert(name,value.toDouble());
        }

        if(name == "Genres")
        {
            QVariantList xs;
            //moram 1 po 1 element da citam iz liste ...
            while (reader.readNextStartElement()) {
                value = reader.readElementText();
                xs.append(value);
            }
           map.insert("Genres",xs);
        }

        if(name == "Year")
        {
            value = reader.readElementText();
            map.insert(name, value.toUInt());
        }
    }

    file.close();

    //azuriraj objekte sa trazenim podacima
    load(map);
}

QString song::getArtist() const
{
    return m_artist;
}

QString song::getTitle() const
{
    return m_title;
}

QString song::getAlbum() const
{
    return m_album;
}

double song::getRating() const
{
    return m_rating;
}

void song::setRating(double rating)
{
    m_rating = rating;
}

void song::saveToFile() const
{
    QFile file{"pesme2.xml"};
    if(!file.open(QFile::WriteOnly))
    {
        cout << "Neuspelo otvaranje fajla za pisanje!" << endl;
        return;
    }

    QXmlStreamWriter writer{&file};
    writer.writeStartDocument();

    QVariantMap map =  save().toMap();
    writer.setAutoFormatting(true);
    writer.writeStartElement("Song");

    //atribut je type = ""
    writer.writeAttribute("type","QVariantMap");
    QMapIterator<QString,QVariant> i{map};
    while(i.hasNext()){
        i.next(); //predji na sledeci element
        auto tip = i.key();

        if(tip == "Artist" || tip == "Album" || tip == "Album"){
            writer.writeStartElement(i.key());
            writer.writeAttribute("type", "QString");
            writer.writeCharacters(i.value().toString());
            writer.writeEndElement();
        }

        if(tip == "Rating")
        {
            writer.writeStartElement(i.key());
            writer.writeAttribute("type", "double");
            writer.writeCharacters(i.value().toString());
            writer.writeEndElement();
        }

        if(tip == "Year")
        {
            writer.writeStartElement(i.key());
            writer.writeAttribute("type", "uint");
            writer.writeCharacters(i.value().toString());
            writer.writeEndElement();
        }

        if(tip == "Features")
        {
            writer.writeStartElement(tip);
            writer.writeAttribute("type","QVariantList");
            QListIterator<QVariant> i{map.value(tip).toList()};
            while(i.hasNext())
            {
            writer.writeStartElement("Feature"); //<Features
            writer.writeAttribute("type", "QString");
            writer.writeCharacters(i.next().toString());
            writer.writeEndElement();
        }
            writer.writeEndElement();
        }

        if(tip == "Genres")
        {
            writer.writeStartElement(tip);
            writer.writeAttribute("type","QVariantList");
            QListIterator<QVariant> i{map.value(tip).toList()};

            while(i.hasNext())
            {
                writer.writeStartElement("Genre"); //<Feature
                writer.writeAttribute("type", "QString");
                writer.writeCharacters(i.next().toString());
                writer.writeEndElement();
            }
            writer.writeEndElement();
        }
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    file.close();
}

QVector<QString> song::getFeatures() const
{
    QVector<QString> f;
    for(auto f : m_features)
        f.push_back(f);
    return f;
}

QVector<QString> song::getGenres() const
{
    QVector<QString> f;
for(auto f : m_genres)
    f.push_back(f);
return f;
}
