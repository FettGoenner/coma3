#include "networkpuzzlefile.h"

NetworkPuzzleFile::NetworkPuzzleFile() :QFile()
{
}

NetworkPuzzleFile::NetworkPuzzleFile(QFile *parent) : QFile(parent)
{
}

NetworkPuzzleFile::NetworkPuzzleFile(QString &fileName)
    : QFile(fileName)
{
}


