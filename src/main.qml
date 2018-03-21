import QtQuick 2.7
import QtQuick.Controls 1.4

import Glocon 1.0
import '../assets/scripts/glocon.js' as Glocon

Item {
  id: screen
  width: 800
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
    consoleBackground.visible = !consoleBackground.visible;
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

      if (event.key === Qt.Key_F1) {
        applicationWindow.close();
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
    id: consoleBackground
    visible: false
    color: Qt.rgba(0, 0, 0, 0.8)
    height: parent.height / 5
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right

    Row {
      anchors.right: consoleBackground.right
      anchors.verticalCenter: consoleBackground.verticalCenter
      height: consoleBackground.height - 40

      Slider {
        id: sliderOne
        stepSize: 0.01
        orientation: Qt.Vertical
        height: parent.height
        onValueChanged: GameState.sendUpdate({"lightOneIntensity": value})
      }

      Slider {
        id: sliderTwo
        stepSize: 0.01
        orientation: Qt.Vertical
        height: parent.height
        onValueChanged: GameState.sendUpdate({"lightTwoIntensity": value})
      }
    }
  }

  Rectangle {
    color: Qt.rgba(0, 0, 0, 0.8)
    height: 55
    width: 480
    anchors.top: parent.top
    anchors.left: parent.left
  }

  // Show the coords and scale
  Column {
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.margins: 10

    Text {
      font.pixelSize: 23;
      font.family: shareTechMono.name
      color: "green"
      text: {
        var point = contentToCenterWorld(flickArea.contentX, flickArea.contentY);
        return 'Worldspace: (' + point.x.toFixed(2) + ', ' + point.y.toFixed(2) +
          ', ' + scaleToWorldDistance().toFixed(2) + ')';
      }
    }
  }

  TextEdit {
    id: consoleInput
    font.family: shareTechMono.name
    font.pixelSize: 18;
    visible: false
    color: "green"
    wrapMode: Text.WordWrap
    text: JSConsole.text
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.top: consoleBackground.top
    anchors.margins: 10
    onTextChanged: JSConsole.text = consoleInput.text
    Component.onCompleted: {
      JSConsole.textChanged.connect(function() {
         consoleInput.text = JSConsole.text;
      });
    }

    Keys.priority: Keys.BeforeItem
    Keys.onPressed: {
      var isControl = false;

      if (event.modifiers & (Qt.ControlModifier | Qt.MetaModifier)) {
        isControl = true;
      }

      if (event.key === Qt.Key_Escape) {
        flickArea.focus = true;
        return false;
      }

      if (event.key === Qt.Key_QuoteLeft) {
        return toggleConsole();
      }

      if (event.key === Qt.Key_Up) {
        consoleInput.text = JSConsole.lastText;
        JSConsole.isDirty = false;
        return;
      }

      if (isControl && event.key === Qt.Key_E) {
        cursorPosition = consoleInput.text.length;
        return;
      }

      if (isControl && event.key === Qt.Key_E) {
        cursorPosition = consoleInput.text.length;
        return;
      }

      if (JSConsole.isDirty) {
        JSConsole.isDirty = false;
        consoleInput.text = '';
      }

      if (isControl && (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)) {
        var output = '';
        try {
          var result = eval(consoleInput.text);
          if (typeof result == 'string') {
            output = result;
          } else {
            output = '[' + typeof result + ']';
          }
        } catch (err) {
          output = 'Error: ' + err.message;
        }

        JSConsole.lastText = consoleInput.text;
        consoleInput.text = output;
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
      font.pixelSize: 32
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
    //onContentRectChanged: flickArea.resizeContent(this.contentRect.width(), this.contentRect.height(), flickArea.Center)
    Component.onCompleted: {
      JSConsole.sayHello.connect(GameState.hello);
      GameState.sendUpdate.connect(renderer.receiveUpdate);

      //JSConsole.onPositionChanged.connect(renderer.updatePosition);
      //JSConsole.updatePosition(renderer.x, renderer.y, renderer.z);
    }
  }
}
