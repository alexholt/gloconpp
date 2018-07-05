var _ = null;
var currentCommand = '';
var output = '';
var prompt = '> ';
var lastCommand = '';
var cursorPosition = prompt.length

function getOutput() {
  return output + prompt + currentCommand;
}

function getPosition() {
  return cursorPosition;
}

function requestPosition(pos) {
  if (pos === Infinity) {
    cursorPosition = (output + prompt + currentCommand).length;
    return;
  }

  if (pos === -Infinity) {
    cursorPosition = (output + prompt).length;
    return;
  }

  // Is it within the text range of the current command?
  // Previous commands are not editable
  if (pos >= (output + prompt).length) {
    cursorPosition = pos;
  }
}

function pressKey(event) {
  var isControl = false;

  if (event.modifiers & (Qt.ControlModifier | Qt.MetaModifier)) {
    isControl = true;
  }

  if (event.key === Qt.Key_Up) {
    currentCommand = lastCommand;
    return;
  }

  if (isControl && event.key === Qt.Key_A) {
    requestPosition(-Infinity);
    return;
  }

  if (isControl && event.key === Qt.Key_E) {
    requestPosition(Infinity);
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
    requestPosition(Infinity);
    event.accepted = true;
    return;
  }

  if (event.key === Qt.Key_Backspace) {
    currentCommand = currentCommand.slice(0, cursorPosition - 3) +
        currentCommand.slice(cursorPosition - 2);
    requestPosition(cursorPosition - 1);
    event.accepted = true;
    return;
  }

  if (event.key === Qt.Key_Left) {
    requestPosition(cursorPosition - 1);
    event.accepted = true;
    return;
  }

  if (event.key === Qt.Key_Right) {
    requestPosition(cursorPosition + 1);
    event.accepted = true;
    return;
  }

  currentCommand += event.text;
  event.accepted = true;
  cursorPosition = getOutput().length;
}
