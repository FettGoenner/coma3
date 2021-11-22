#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QVector>

class Node : public QWidget
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr);
    void rotate90(); // rotate the node 90 degrees clockwise
    QVector<bool> orientation = {0, 0, 0, 0}; // shows where the ways are
    QColor nodeColor;
    int rotateAngle = 0; // the angle that this node already rotated
protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
private:

signals:
    void clicked();

};

#endif // NODE_H
