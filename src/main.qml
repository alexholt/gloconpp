import QtQuick 2.7
import QtQuick.Controls 1.4

import Glocon 1.0
import '../assets/scripts/glocon.js' as Glocon

Item {
  id: screen
  width: 1000
  height: 800

  FontLoader {
    id: shareTechMono
    source: "../assets/fonts/ShareTechMono-Regular.ttf"
  }

  // Hook up currentScale, mapCenterX and mapCenterY to the map renderer
  // Assuming that (0, 0) is the center of the content
  property real currentScale: 1 + Math.pow(zoomStep * 0.01, 2.0) + 0.01   // 0.01 is a workaround for unwanted Flickable behavior
  property real mapCenterX: flickArea.contentX + screen.width * 0.5 - mapWidth * 0.5
  property real mapCenterY: flickArea.contentY + screen.height * 0.5 - mapHeight * 0.5

  property int zoomStep: 0
  property int maxZoomStep: 500

  // These are the width and height of the pan-zoomed content at scale 1.0 (which is also the minimum)
  // You should set these yourself. I'm taking them from the dummy image here.
  property int mapWidth: 2000
  property int mapHeight: 1000

  function resizeMap(x, y, scaleUp) {
    zoomStep = scaleUp ? Math.min(maxZoomStep, zoomStep + 1) :
      Math.max(0.0, zoomStep - 1);
    flickArea.resizeContent(mapWidth * currentScale, mapHeight * currentScale, Qt.point(x, y));
    flickArea.returnToBounds()
  }

  // ContentX and contentY are the top left corner
  function contentToCenterWorld(x, y) {
    var width = screen.width;
    var height = screen.height;
    var scale = currentScale;
    x += width / 2;
    y += height / 2;
    x /= scale;
    y /= scale;
    return Qt.point(x, y);
  }

  function scaleToWorldDistance() {
    var aspect = screen.width / screen.height;
    var fov = 2 * Math.PI * 60 / 360;
    var fullSize = -screen.width / (aspect * Math.tan(fov / 2));
    return fullSize / currentScale * 0.5;
  }

  function toggleConsole() {
    consoleInput.visible = !consoleInput.visible;
    flickArea.focus = !consoleInput.visible;
    consoleInput.focus = consoleInput.visible;
    return true;
  }

  Flickable {
    id: flickArea
    focus: true
    boundsBehavior: Flickable.DragAndOvershootBounds
    anchors.fill: parent
    interactive: true
    contentWidth: 2000
    contentHeight: 1000
    Component.onCompleted: {
      renderer.updatePosition(0, 0, currentScale);
    }
    Keys.onPressed: {
      if (event.key === Qt.Key_Escape) {
        consoleInput.focus = true;
        return false;
      }

      if (event.key === Qt.Key_QuoteLeft) {
        return toggleConsole();
      }

      renderer.onKeyPressed(event.key);
    }
    onContentXChanged: {
      var point = contentToCenterWorld(contentX, contentY) ;
      renderer.updatePosition(point.x, point.y, currentScale);
    }
    onContentYChanged: {
      var point = contentToCenterWorld(contentX, contentY) ;
      renderer.updatePosition(point.x, point.y, currentScale);
    }
  }

  MouseArea {
    anchors.fill: parent
    onWheel: {
      resizeMap(flickArea.contentX + wheel.x, flickArea.contentY + wheel.y, wheel.angleDelta.y > 0);
    }
    onPressed: mouse.accepted = false
    onReleased: mouse.accepted = false
  }

  Rectangle {
    color: Qt.rgba(0, 0, 0, 0.8)
    height: 40
    width: 420
    anchors.top: parent.top
    anchors.left: parent.left

    Text {
      font.pixelSize: 18;
      font.family: shareTechMono.name
      color: "green"
      anchors.centerIn: parent

      text: {
        var point = contentToCenterWorld(flickArea.contentX, flickArea.contentY);
        return 'Worldspace: (' + point.x.toFixed(2) + ', ' + point.y.toFixed(2) +
          ', ' + scaleToWorldDistance().toFixed(2) + ')';
      }
    }
  }

  Rectangle {
    color: Qt.rgba(0, 0, 0, 0.8)
    height: 40
    width: 60
    anchors.top: parent.top
    anchors.right: parent.right

    Text {
      color: "green"
      text: renderer.fps
      anchors.centerIn: parent
      font.pixelSize: 18
      font.family: shareTechMono.name
    }
  }

  Renderer {
    id: renderer
    property real currentScale: Math.pow(zoomStep * 0.01, 2.0) + 0.01
    property real mapCenterX: flickArea.contentX + screen.width * 0.5 - mapWidth * 0.5
    property real mapCenterY: flickArea.contentY + screen.height * 0.5 - mapHeight * 0.5

    property int zoomStep: 100
    property int maxZoomStep: 100

    property int mapWidth: 2000
    property int mapHeight: 1000

    window: applicationWindow // applicationWindow added to the global object in main.cpp
    Component.onCompleted: {
      JSConsole.sayHello.connect(GameState.hello);
      GameState.sendUpdate.connect(renderer.receiveUpdate);
    }
  }

  Console {
    id: consoleInput
  }
}
