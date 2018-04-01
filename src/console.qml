import QtQuick 2.7
import QtQuick.Controls 1.4

import Glocon 1.0

Rectangle {
  height: screen.height - handle.y
  color: 'red'

  ScrollView {
    id: rect
    y: handle.y
    width: screen.width
    height: screen.height - handle.y

    Rectangle {
      color: 'red'

      //TextEdit {
      //  id: textInput
      //  font.family: shareTechMono.name
      //  font.pixelSize: 18;
      //  visible: false
      //  color: "green"
      //  wrapMode: Text.WordWrap
      //  text: JSConsole.text
      //  anchors.margins: 10
      //  onTextChanged: JSConsole.text = textInput.text
      //  Component.onCompleted: {
      //    JSConsole.textChanged.connect(function() {
      //      text = JSConsole.text;
      //    });
      //  }

      //  Keys.priority: Keys.BeforeItem
      //  Keys.onPressed: {
      //    var isControl = false;

      //    if (event.modifiers & (Qt.ControlModifier | Qt.MetaModifier)) {
      //      isControl = true;
      //    }

      //    if (event.key === Qt.Key_Escape) {
      //      flickArea.focus = true;
      //      return false;
      //    }

      //    if (event.key === Qt.Key_QuoteLeft) {
      //      return toggleConsole();
      //    }

      //    if (event.key === Qt.Key_Up) {
      //      textInput.text = JSConsole.lastText;
      //      JSConsole.isDirty = false;
      //      return;
      //    }

      //    if (isControl && event.key === Qt.Key_E) {
      //      cursorPosition = textInput.text.length;
      //      return;
      //    }

      //    if (isControl && event.key === Qt.Key_E) {
      //      cursorPosition = textInput.text.length;
      //      return;
      //    }

      //    if (JSConsole.isDirty) {
      //      JSConsole.isDirty = false;
      //      textInput.text = '';
      //    }

      //    if (isControl && (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)) {
      //      var output = '';
      //      try {
      //        var result = eval(textInput.text);
      //        if (typeof result == 'string') {
      //          output = result;
      //        } else {
      //          output = '[' + typeof result + ']';
      //        }
      //      } catch (err) {
      //        output = 'Error: ' + err.message;
      //      }

      //      JSConsole.lastText = textInput.text;
      //      textInput.text = output;
      //      cursorPosition = textInput.text.length;
      //      JSConsole.isDirty = true;
      //      return false;
      //    }
      //  }
      //}
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
      drag.minimumY: 0; drag.maximumY: 800
    }
  }
}
