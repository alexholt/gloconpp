import QtQuick 2.0
import GloconPP 1.0

Item {

    width: 700
    height: 800

    Viewport {
        id: viewport
    }

    Rectangle {
        id: consoleBackground
        color: Qt.rgba(0, 0, 0, 0.4)
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

    Text {
      text: viewport.fps
      anchors.top: parent.top
      anchors.right: parent.right
      font.family: "PT Mono"
    }

}
