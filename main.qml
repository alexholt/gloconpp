import QtQuick 2.0
import Glocon 1.0

Item {
  width: 800
  height: 800

  MouseArea {
    anchors.fill: parent
    onWheel: renderer.zoom(wheel.angleDelta)

    Flickable {
      id: flickArea
      focus: true
      //boundsBehavior: Flickable.StopAtBounds
      anchors.fill: parent
      contentX: 0
      contentY: 0
      Keys.onPressed: renderer.onKeyPressed(event.key)
      onContentXChanged: {
        this.focus = true
        console.log(contentX)
        renderer.onPanX(contentX)
      }
      onContentYChanged: {
        this.focus = true
        renderer.onPanY(contentY)
      }
    }
  }

  Renderer {
    id: renderer
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
