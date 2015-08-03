// This custom element was originally presented in
//
// http://www.developer.nokia.com/Community/Wiki/CS001627_-_Creating_a_custom_QML_element_with_Qt
//
// It was modified into separated line.h/line.cpp and modified for Qt5
// by Julius Luukko
//

#ifndef LINE_H
#define LINE_H
 
#include <QQuickPaintedItem>
#include <QPainter>
#include <QPen>
 
class Line : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int x1 READ x1 WRITE setX1 NOTIFY x1Changed);
    Q_PROPERTY(int y1 READ y1 WRITE setY1 NOTIFY y1Changed);
    Q_PROPERTY(int x2 READ x2 WRITE setX2 NOTIFY x2Changed);
    Q_PROPERTY(int y2 READ y2 WRITE setY2 NOTIFY y2Changed);
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged);
    Q_PROPERTY(int penWidth READ penWidth WRITE setPenWidth NOTIFY penWidthChanged);
 
public:
    Line(QQuickPaintedItem *parent = 0);

    void paint(QPainter *painter);
 
    // Get methods
    int x1() const { return m_x1; }
    int y1() const { return m_y1; }
    int x2() const { return m_x2; }
    int y2() const { return m_y2; }
    QColor color() const { return m_color; }
    int penWidth() const { return m_penWidth; }
 
    // Set methods
    void setX1(int x1);
    void setY1(int y1);
    void setX2(int x2);
    void setY2(int y2);
    void setColor(const QColor &color);
    void setPenWidth(int newWidth);
 
signals:
    void x1Changed();
    void y1Changed();
    void x2Changed();
    void y2Changed();
    void colorChanged();
    void penWidthChanged();
 
protected:
    void updateSize();
 
protected:
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;
    QColor m_color;
    int m_penWidth;
};
 
QML_DECLARE_TYPE(Line)
 
#endif // LINE_H
