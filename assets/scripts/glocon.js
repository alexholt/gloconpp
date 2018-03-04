var Glocon = {};

Glocon.x = 0;
Glocon.y = 0;
Glocon.z = 0;

Glocon.toString = function () {
  var output = '';
  for (var prop in this) {
    output += (prop + ': ' + typeof this[prop] + '\n');
  }
  return output;
}

Glocon.moveTo = function (x, y, z) {
  this.x = x;
  this.y = y;
  this.z = z;
};

Glocon.resetCamera = function () {
  this.moveTo(0, 0, -1000);
  return 'Camera moved';
};

Glocon.hello = function () {
  return "Hello";
};

function hello() {
  return "hello";
}
