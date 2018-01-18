#ifndef RECT_H
#define RECT_H

#include <QObject>

class Rect : public QObject
{
  Q_OBJECT

public:
  Rect();
  Rect(float, float, float, float);
  Rect(const Rect&);
  void setX(float);
  float x() const;
  void setY(float);
  float y() const;
  void setX2(float);
  float x2() const;
  void setY2(float);
  float y2() const;
  float width() const;
  float height() const;

private:
  float m_x = std::numeric_limits<float>::max();
  float m_y = std::numeric_limits<float>::max();
  float m_x2 = std::numeric_limits<float>::min();
  float m_y2 = std::numeric_limits<float>::min();

signals:

public slots:
};

#endif // RECT_H
