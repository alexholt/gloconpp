#include "territory.h"

#include <QVector3D>

Territory::Territory(QString path) {
  m_path = path;
}

Territory::~Territory() {
  if (m_centroid != nullptr)
    delete m_centroid;

  for (int i = 0; i < m_mesh.length(); i++)
    delete m_mesh[i];
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

QRect Territory::getBoundingBox() {
  if (m_boundingBox.width() > 0) {
    return m_boundingBox;
  }

  qint32 max = quint32(0 - 1);
  qint32 min = 0;

  m_boundingBox.setCoords(max, max, min, min);
  m_lastPoint->setX(0);
  m_lastPoint->setY(0);
  m_pathState = BEGIN;
  m_currentVal = NAN;
  getPointArray();

  QList<QVector2D*>::const_iterator i = m_pointArray.constBegin();

  while (i != m_pointArray.constEnd()) {
    QVector2D* cur = *i;

    if (m_boundingBox.x() > cur->x()) {
      m_boundingBox.setX(cur->x());
    }

    if (m_boundingBox.y() > cur->y()) {
      m_boundingBox.setY(cur->y());
    }

    if (m_boundingBox.right() < cur->x()) {
      m_boundingBox.setWidth(cur->x() - m_boundingBox.x());
    }

    if (m_boundingBox.bottom() < cur->y()) {
      m_boundingBox.setHeight(cur->y() - m_boundingBox.y());
    }
  }

  return m_boundingBox;
}

// Formula is from https://en.wikipedia.org/wiki/Centroid, duh
// @return {Point}
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

QList<QVector3D*>& Territory::getMesh() {
  auto vertices = getPointArray();
  if (m_mesh.length() > 0)
    return m_mesh;

  // Add super triangle to mesh
  auto box = getBoundingBox();
  QList<QVector3D> superTriangle;

  // Top
  superTriangle.append(
    QVector3D{
      static_cast<float>(box.bottomLeft().x()),
      static_cast<float>(box.topLeft().y() + box.height()),
      0.0f
    }
  );

  // Bottom Left
  superTriangle.append(
    QVector3D{
      static_cast<float>(box.bottomLeft().x()),
      static_cast<float>(box.bottomLeft().y()),
      0.0f
    }
  );

  // Bottom Right
  superTriangle.append(
    QVector3D{
      static_cast<float>(box.bottomRight().x() + box.width()),
      static_cast<float>(box.bottomLeft().y()),
      0.0f
    }
  );

  QList<QList<QVector3D>> triangles;
  triangles.append(superTriangle);

  for (int i = 0; i < vertices.length(); i++) {
    QVector3D point{vertices[i]->x(), vertices[i]->y(), 0.0f};
    QList<QList<QVector3D>> badTriangles;

    //for (auto triangle : triangles) {
    //  if (circumCircle(triangle).contains(point))
    //    badTriangles.append(triangle);
    //}

    //QList<QList<QVector3D>> mesh;
    //for (auto triangle : badTriangles) {
    //  for (auto edge : triangle) {
    //    if (!badTriangles.contains(edge)) {
    //      mesh.append(edge);
    //    }
    //  }
    //}

    //for (auto triangle : badTriangles) {
    //  triangles.remove(triangle);
    //}

    //for (auto edge : mesh) {
    //  QList<QVector3D> edgeToPoint{
    //    QVector3D{edge[0].x(), edge[0].y(), 0.0f},
    //    QVector3D{edge[0].x2(), edge[0].y2(), 0.0f},
    //    QVector3D{point.x(), point().y(), 0.0f},
    //  };

    //  triangles.append(edgeToPoint);
    //}

  }

  //for (auto triangle : triangles) {
  //   if (superTriangle.contains(triangle))
  //      triangles.remove(triangle)
  //}


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
