glocon.x = 0;
glocon.y = 0;
glocon.z = 0;

glocon.toString = function () {
  return JSON.stringify(this);
}

glocon.moveTo = function (x, y, z) {
  this.x = x;
  this.y = y;
  this.z = z;
};

glocon.resetCamera = function () {
  this.moveTo(0, 0, -1000);
};
