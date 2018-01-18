#ifndef EDGE_H
#define EDGE_H

#include <QObject>
#include <QVector3D>

class Edge : public QObject
{
  Q_OBJECT
public:
  Edge();
  Edge(const Edge&);
  Edge(const QVector3D&, const QVector3D&);
  const QVector3D& secondPoint() const;
  const QVector3D& firstPoint() const;
  bool operator ==(const Edge&) const;
  Edge operator =(const Edge&) const;

private:
  QVector3D m_firstPoint;
  QVector3D m_secondPoint;

signals:

public slots:
};

#endif // EDGE_H
