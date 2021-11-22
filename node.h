#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QVector>

class Node : public QWidget
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr);
    void rotate90();
    QVector<bool> orientation = {0, 0, 0, 0};
    QColor nodeColor;
    int rotateAngle = 0;
protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
private:

signals:
    void clicked();

};

#endif // NODE_H
