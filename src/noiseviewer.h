#ifndef NOISEVIEWER_H
#define NOISEVIEWER_H

#include <QQuickPaintedItem>

class NoiseViewer : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(int width READ width WRITE setWidth)
  Q_PROPERTY(int height READ height WRITE setHeight)

public:
  NoiseViewer();

signals:

public slots:
  void paint(QPainter*) override;
};

#endif // NOISEVIEWER_H
