import QtQuick 2.7
import QtQuick.Controls 1.4

import Glocon 1.0

import '../assets/scripts/Buffer.js' as Buffer

Rectangle {
  id: container
  width: parent.width
  height: parent.height
  color: 'transparent'

  onFocusChanged: {
    if (container.focus) {
      textInput.forceActiveFocus();
    }
  }

  ScrollView {
    id: rect
    width: parent.width
    height: container.height - handle.y
    y: handle.y

    Rectangle {
      id: textInputBackground
      color: Qt.rgba(0, 0, 0, 0.8)
      width: container.width
      height: Math.max(textInput.height - 20, rect.height - 10)

      MouseArea {
        anchors.fill: parent
        onClicked: textInput.forceActiveFocus()

        TextEdit {
          id: textInput
          font.family: shareTechMono.name
          font.pixelSize: 18;
          color: "green"
          focus: true
          wrapMode: Text.WordWrap

          onCursorPositionChanged: {
            Buffer.requestPosition(cursorPosition)
            cursorPosition = Buffer.getPosition()
          }

          Component.onCompleted: {
            text = Buffer.getOutput()
            cursorPosition = Buffer.getPosition()
          }

          padding: 20

          Keys.onPressed: {
            if (event.key === Qt.Key_Escape) {
              flickArea.focus = true;
              return false;
            }

            if (event.key === Qt.Key_QuoteLeft) {
              event.accepted = true;
              return toggleConsole();
            }

            Buffer.pressKey(event);
            text = Buffer.getOutput();
            cursorPosition = Buffer.getPosition();
          }

        }
      }
    }
  }


  Rectangle {
    id: handle
    height: 10
    width: parent.width

    MouseArea {
      anchors.fill: parent
      drag.axis: Drag.YAxis
      drag.target: handle
      drag.minimumY: 0
      drag.maximumY: 800
      cursorShape: Qt.SizeVerCursor
    }
  }
}
