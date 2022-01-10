#ifndef SAVEGAMECONTROLER_H
#define SAVEGAMECONTROLER_H

#include <QObject>

class QMouseEvent;
class QKeyEvent;

class SaveFileModel;
class SaveGameDialog;

class savegamecontroler
{
    Q_OBJECT

    // MVC: Die Steuerung kennt sowohl das Modell als auch die Ansicht
    // Referenzen auf die beiden Objekte (keine Kopie oder eigene Objekte!)
    SaveFileModel& saveGame;
    SaveGameDialog& m_view;
public:
    savegamecontroler(SaveFileModel& model, SaveGameDialog& view, QObject* parent = nullptr);
};

#endif // SAVEGAMECONTROLER_H
