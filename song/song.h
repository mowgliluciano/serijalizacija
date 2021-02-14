#ifndef SONG_H
#define SONG_H

#include <QVariant>
#include <QString>
#include <QVector>
#include <QVariantMap>
#include <QVariantList>

class song
{
public:
    explicit song() = default;

    void load(QVariant data);
    QVariant save() const;
    void deserialize();
    QString getArtist()const;
    QString getTitle()const;
    QString getAlbum()const;
    double getRating()const;
    void setRating(double rating);
    void saveToFile() const;
    QVector <QString> getFeatures() const;

    QVector <QString> getGenres() const;
private:
    QString m_artist;
    QString m_title;
    QString m_album;
    QVector<QString> m_features;
    double m_rating;
    QVector<QString> m_genres;
    unsigned m_year;
};

#endif // SONG_H
