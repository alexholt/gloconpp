import QtQuick 2.7
import Glocon 1.0

Item {
  id: screen
  width: 800
  height: 800

  // Zoom with up/down arrows
  focus: true
  Keys.onUpPressed: resizeMap(flickArea.contentX + screen.width * 0.5, flickArea.contentY + screen.height * 0.5, true)
  Keys.onDownPressed: resizeMap(flickArea.contentX + screen.width * 0.5, flickArea.contentY + screen.height * 0.5, false)

  function resizeMap(x, y, scaleUp) {
    var mapWidth = renderer.mapWidth;
    var mapHeight = renderer.mapHeight;
    var currentScale = renderer.currentScale;
    var maxZoomStep = renderer.maxZoomStep;
    var zoomStep = renderer.zoomStep;

    renderer.zoomStep = scaleUp ? Math.min(maxZoomStep, zoomStep + 1) :
                         Math.max(0.0, zoomStep - 1);
    flickArea.resizeContent(mapWidth * currentScale, mapHeight * currentScale, Qt.point(x, y));
    flickArea.returnToBounds()
    renderer.updatePosition(renderer.mapCenterX, renderer.mapCenterY, renderer.currentScale);
  }


  Flickable {
    id: flickArea
    focus: true
    boundsBehavior: Flickable.DragAndOvershootBounds
    anchors.fill: parent
    interactive: true
    contentWidth: 2000
    contentHeight: 1000
    Keys.onPressed: {
      if (event.key === Qt.Key_Escape) {
        consoleInput.focus = true;
        return false;
      }

      if (event.key === Qt.Key_F1) {
        applicationWindow.close();
        return false;
      }

      renderer.onKeyPressed(event.key);
    }
    onContentXChanged: {
      renderer.updatePosition(renderer.mapCenterX, renderer.mapCenterY, renderer.currentScale);
    }
    onContentYChanged: {
      renderer.updatePosition(renderer.mapCenterX, renderer.mapCenterY, renderer.currentScale);
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

  Renderer {
    id: renderer
    property real currentScale: Math.pow(zoomStep * 0.1, 2.0) + 0.01
    property real mapCenterX: flickArea.contentX + screen.width * 0.5 - mapWidth * 0.5
    property real mapCenterY: flickArea.contentY + screen.height * 0.5 - mapHeight * 0.5

    property int zoomStep: 0
    property int maxZoomStep: 50

    property int mapWidth: 2000
    property int mapHeight: 1000

    window: applicationWindow // applicationWindow added to the global object in main.cpp
    onContentRectChanged: flickArea.resizeContent(this.contentRect.width(), this.contentRect.height(), flickArea.Center)
    Component.onCompleted: {
      JSConsole.onPositionChanged.connect(renderer.updatePosition);
      JSConsole.updatePosition(renderer.x, renderer.y, renderer.z);
    }
  }

  Rectangle {
    id: consoleBackground
    color: Qt.rgba(0, 0, 0, 0.8)
    height: parent.height / 5
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
  }


  Rectangle {
    color: Qt.rgba(0, 0, 0, 0.8)
    height: 55
    width: 300
    anchors.top: parent.top
    anchors.left: parent.left
  }

  // Show the coords and scale
  Column {
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.margins: 10

    Text {
      text: 'Center of map: (' + renderer.mapCenterX.toFixed(2) + ', ' +
        renderer.mapCenterY.toFixed(2) + ')' + '\n' + 'Scale: ' + renderer.currentScale.toFixed(2)
      font.family: "PT Mono"
      font.pixelSize: 16
      color: "green"
    }
  }

  TextEdit {
    id: consoleInput

    color: "green"
    wrapMode: Text.WordWrap
    text: JSConsole.text
    font.family: "PT Mono"
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.top: consoleBackground.top
    anchors.margins: 10
    onTextChanged: JSConsole.text = consoleInput.text

    Keys.priority: Keys.BeforeItem
    Keys.onPressed: {
      if (event.key === Qt.Key_Escape) {
        flickArea.focus = true;
        return false;
      }

      if (JSConsole.isDirty) {
        JSConsole.isDirty = false;
        consoleInput.text = '';
      }

      if (
        (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) &&
        (event.modifiers & Qt.ControlModifier)
      ) {
        consoleInput.text = JSConsole.evaluate();
        cursorPosition = consoleInput.text.length;
        JSConsole.isDirty = true;
        return false;
      }
    }
  }

  Rectangle {
    color: Qt.rgba(0, 0, 0, 0.8)
    height: 60
    width: 60
    anchors.top: parent.top
    anchors.right: parent.right

    Text {
      color: "green"
      text: renderer.fps
      anchors.centerIn: parent
      font.family: "PT Mono"
      font.pixelSize: 32
    }
  }
}
