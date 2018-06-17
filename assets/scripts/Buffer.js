var _ = null;
var currentCommand = '';
var output = '';
var prompt = '> ';
var lastCommand = '';

function getOutput() {
  return output + prompt + currentCommand;
}

function getPosition() {
  return getOutput().length;
}

function pressKey(event) {
  var isControl = false;

  if (event.modifiers & (Qt.ControlModifier | Qt.MetaModifier)) {
    isControl = true;
  }

  if (event.key === Qt.Key_Up) {
    return;
  }

  if (isControl && event.key === Qt.Key_E) {
    //cursorPosition = textInput.text.length;
    return;
  }

  if (isControl && event.key === Qt.Key_E) {
    currentCommand = lastCommand;
    return;
  }

  if (isControl && (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)) {
    output += prompt + currentCommand + '\n';

    try {
      var result = eval(currentCommand);
      lastCommand = currentCommand;
      _ = result;
      if (typeof result == 'string') {
        output += result + '\n';
      } else if (typeof result == 'number') {
        output += String(result) + '\n';
      } else {
        output += '[' + typeof result + ']\n';
      }
    } catch (err) {
      output += 'Error: ' + err.message + '\n';
    }

    currentCommand = '';
    event.accepted = true
    return;
  }

  if (event.key === Qt.Key_Backspace) {
    currentCommand = currentCommand.slice(0, -1)
    event.accepted = true
    return;
  }

  currentCommand += event.text
  event.accepted = true
}
