import QtQuick 2.0
import GloconPP 1.0

Item {

  width: 700
  height: 800

  MouseArea {
    anchors.fill: parent
    onWheel: renderer.zoom(wheel.delta)

    Flickable {
      focus: true
      //boundsBehavior: Flickable.StopAtBounds
      anchors.fill: parent
      contentWidth: 2000
      contentHeight: 500
      Keys.onPressed: renderer.onKeyPressed(event.key)

      onContentXChanged: renderer.onPanX(contentX)
      onContentYChanged: renderer.onPanY(contentY)
    }
  }

  Renderer {
    id: renderer
    window: applicationWindow // applicationWindow added to the global object in main.cpp
  }

  Rectangle {
    id: consoleBackground
    color: Qt.rgba(0, 0, 0, 0.8)
    height: parent.height / 5
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
  }

  JSConsole {
    id: jsConsole
  }

  TextEdit {
    id: consoleInput

    color: "green"
    wrapMode: Text.WordWrap
    text: jsConsole.text
    font.family: "PT Mono"
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.top: consoleBackground.top
    anchors.margins: 10
    onTextChanged: jsConsole.text = consoleInput.text

    Keys.priority: Keys.BeforeItem
    Keys.onPressed: {
      if (
          (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) &&
          (event.modifiers & Qt.ControlModifier)
          ) {
        consoleInput.text = jsConsole.evaluate();
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
