#include <limits>
#include <QVector3D>

#include "circle.h"
#include "edge.h"
#include "territory.h"
#include "triangle.h"

Territory::Territory(QString path) {
  m_path = path;
}

Territory::~Territory() {
  if (m_centroid != nullptr)
    delete m_centroid;
}

void Territory::processToken(QString token) {
  token = token.toLower();

  switch (token[0].unicode()) {
  case 'm':
  case 'v':
  case 'h':
  case 'z':
  case ',':
    stateChange(token);
    return;

  default:
    const float val = token.toFloat();
    QVector2D* point = new QVector2D;

    switch (m_pathState) {
    case BEGIN:
    case CLOSE_PATH:
      break;

    case MOVE:
      m_pathState = MOVE_X;
      m_currentVal = val + m_lastPoint->x();
      break;

    case MOVE_X:
      point->setX(m_currentVal);
      point->setY(val + m_lastPoint->y());
      m_pointArray << point;
      m_currentVal = NAN;
      m_lastPoint = m_pointArray.last();
      m_pathState = MOVE;
      break;

    case MOVE_H:
      m_lastPoint = m_pointArray.last();
      point->setX(m_lastPoint->x() + val);
      point->setY(m_lastPoint->y());
      m_pointArray << point;
      break;

    case MOVE_V:
      m_lastPoint = m_pointArray.last();
      point->setX(m_lastPoint->x());
      point->setY(m_lastPoint->y() + val);
      m_pointArray << point;
      break;
    }
  }
}

void Territory::stateChange(QString token) {
  switch (token[0].unicode()) {
  case 'm':
    m_pathState = MOVE;
    if (m_pointArray.length() > 0) {
      m_lastPoint = m_pointArray.last();
    } else {
      m_lastPoint->setX(0);
      m_lastPoint->setY(0);
    }
    break;
  case 'h':
    m_pathState = MOVE_H;
    break;
  case 'v':
    m_pathState = MOVE_V;
    break;
  case 'z':
    m_pathState = CLOSE_PATH;
    break;
  default:
    QString msg("Unknown path token: ");
    msg += token;
    throw std::runtime_error(msg.toStdString());
  }
}

QList<QVector2D*>& Territory::getPointArray() {
  if (m_pointArray.length() > 0) return m_pointArray;

  m_lastPoint->setX(0);
  m_lastPoint->setY(0);
  m_pathState = BEGIN;
  m_currentVal = NAN;
  QString token;

  for (int i = 0; i < m_path.length(); i++) {
    auto chr = m_path[i];
    if (chr == ' ' || chr == ',') {
      processToken(token);
      token = "";
    } else {
      token += chr;
    }
  }

  return m_pointArray;
}

Rect Territory::getBoundingBox() {
  if (m_boundingBox.width() > 0) {
    return m_boundingBox;
  }

  getPointArray();

  for (QVector2D* cur : m_pointArray) {
    if (m_boundingBox.x() > cur->x()) {
      m_boundingBox.setX(cur->x());
    }

    if (m_boundingBox.y() > cur->y()) {
      m_boundingBox.setY(cur->y());
    }

    if (m_boundingBox.x2() < cur->x()) {
      m_boundingBox.setX2(cur->x());
    }

    if (m_boundingBox.y2() < cur->y()) {
      m_boundingBox.setY2(cur->y());
    }
  }

  return m_boundingBox;
}

Triangle Territory::getSuperTriangle() {
  auto box = getBoundingBox();
  Triangle superTriangle{
    QVector3D{
      box.x() - 1.0f,
      box.y() - 1.0f,
      0.0f
    },
    QVector3D{
      box.x() - 1.0f,
      box.y2() + box.height() + 1.0f,
      0.0f
    },
    QVector3D{
      box.x2() + box.width() + 1.0f,
      box.y2() + box.height() + 1.0f,
      0.0f
    }
  };

  return superTriangle;
}

// Formula is from https://en.wikipedia.org/wiki/Centroid, duh
QVector2D* Territory::getCentroid() {
  if (m_centroid != nullptr) return m_centroid;

  std::function<QList<QVector2D>(QList<QVector2D*>&, int)> nextTwo = [](QList<QVector2D*>& vertices, int i) {
    return QList<QVector2D>{
      QVector2D(vertices[i]->x(), vertices[i]->y()),
      QVector2D(vertices[(i + 1) % vertices.length()]->x(), vertices[(i + 1) % vertices.length()]->y())
    };
  };

  auto vertices = getPointArray();

  // a = 1/2 * sum( xi * yi1 - xi1 * yi )
  float a = 0;
  for (int i = 0; i < vertices.length(); i++) {
    auto points = nextTwo(vertices, i);
    a += points[0].x() * points[1].y() - points[1].x() * points[0].y();
  }

  a /= 2;

  // cx = 1/6a * sum( (xi + xi1) * (xi * yi1 - xi1 * yi) )
  float cx = 0;
  for (int i = 0; i < vertices.length(); i++) {
    auto points = nextTwo(vertices, i);
    cx += (points[0].x() + points[1].x()) * (points[0].x() * points[1].y() - points[1].x() * points[0].y());
  }
  cx *= 1 / (6 * a);

  // cy = 1/6a * sum( (yi + yi1) * (xi * yi1 - xi1 * yi) )
  float cy = 0;
  for (int i = 0; i < vertices.length(); i++) {
    auto points = nextTwo(vertices, i);
    cy += (points[0].y() + points[1].y()) * (points[0].x() * points[1].y() - points[1].x() * points[0].y());
  }
  cy *= 1 / (6 * a);

  m_centroid = new QVector2D{cx, cy};
  return m_centroid;
}

QList<QVector3D>& Territory::getMesh() {
  auto vertices = getPointArray();

  if (m_mesh.length() > 0)
    return m_mesh;

  QList<Triangle> triangulation;

  Triangle superTriangle = getSuperTriangle();
  triangulation.append(superTriangle);

  for (int i = 0; i < vertices.length(); i++) {
    QVector3D point{vertices[i]->x(), vertices[i]->y(), 0.0f};
    QList<Triangle> badTriangles;

    for (auto triangle : triangulation) {
      if (Circle(triangle).contains(point))
        badTriangles.append(triangle);
    }

    QList<QVector3D> polygon;

    for (auto triangle : badTriangles) {
      QList<Edge> edges;
      edges << triangle.topEdge();
      edges << triangle.bottomEdge();
      edges << triangle.leftEdge();

      for (auto edge : edges) {
        bool shouldAdd = false;

        for (auto otherTri : badTriangles) {
          if (triangle == otherTri)
            shouldAdd = shouldAdd || true;
          else
            shouldAdd = shouldAdd || !otherTri.sharesEdge(edge);
        }

        if (shouldAdd)
          polygon << edge.firstPoint() << edge.secondPoint();
      }
    }

    for (auto triangle : badTriangles) {
      qDebug() << "removing" << triangle.bottom() << triangle.top() << triangle.left();
      triangulation.removeAll(triangle);
    }

    for (int i = 0; i < polygon.length(); i += 2) {
      auto firstPoint = polygon[i];
      auto secondPoint = polygon[i + 1];

      Triangle edgeToPoint{
        QVector3D{firstPoint.x(), firstPoint.y(), 0.0f},
        QVector3D{secondPoint.x(), secondPoint.y(), 0.0f},
        QVector3D{point.x(), point.y(), 0.0f}
      };

      triangulation.append(edgeToPoint);
    }

  }

  for (auto triangle : triangulation) {
    if (!superTriangle.sharesVertex(triangle))
      m_mesh << QVector3D(triangle.bottom()) << QVector3D(triangle.top()) << QVector3D(triangle.left());
  }

  return m_mesh;
  /*
  for each point in pointList do // add all the points one at a time to the triangulation
    badTriangles := empty set
    for each triangle in triangulation do // first find all the triangles that are no longer valid due to the insertion
       if point is inside circumcircle of triangle
          add triangle to badTriangles
    polygon := empty set
    for each triangle in badTriangles do // find the boundary of the polygonal hole
       for each edge in triangle do
          if edge is not shared by any other triangles in badTriangles
             add edge to polygon
    for each triangle in badTriangles do // remove them from the data structure
       remove triangle from triangulation
    for each edge in polygon do // re-triangulate the polygonal hole
       newTri := form a triangle from edge to point
       add newTri to triangulation
  for each triangle in triangulation // done inserting points, now clean up
     if triangle contains a vertex from original super-triangle
        remove triangle from triangulation
  return triangulation
  */
}
