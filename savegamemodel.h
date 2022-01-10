#ifndef SAVEGAMEMODEL_H
#define SAVEGAMEMODEL_H

#include <QObject>
#include <QVector>

class SaveGameModel : public QObject
{
    Q_OBJECT
public:
    explicit SaveGameModel(QObject *parent = nullptr);
private:

signals:

};

#endif // SAVEGAMEMODEL_H
