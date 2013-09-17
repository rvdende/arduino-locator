function Quaternion(w, x, y, z) {
	if (w) {this.w = w} else {this.w = 0.0}
	if (x) {this.x = x} else {this.x = 0.0}
	if (y) {this.y = y} else {this.y = 0.0}
	if (z) {this.z = z} else {this.z = 0.0}	
}

Quaternion.prototype.multiply = function(q2) {
	var outq = new Quaternion();	
	outq.w = this.w*q2.w - this.x*q2.x - this.y*q2.y - this.z*q2.z
	outq.x = this.w*q2.x + this.x*q2.w + this.y*q2.z - this.z*q2.y
	outq.y = this.w*q2.y - this.x*q2.z + this.y*q2.w + this.z*q2.x
	outq.z = this.w*q2.z + this.x*q2.y - this.y*q2.x + this.z*q2.w
	return outq;
} 

Quaternion.prototype.length = function() {
	return Math.sqrt(this.x*this.x + this.y*this.y + this.z*this.z + this.w*this.w)
}

Quaternion.prototype.normalize = function() {
	this.x /= this.length()
	this.y /= this.length()
	this.z /= this.length()
	this.w /= this.length()
}

Quaternion.prototype.setFromYPR = function ( yaw,  pitch,  roll)
{
	///360*Math.PI*2
	
    yaw /= 360*Math.PI*2;
    pitch /= 360*Math.PI*2;
    roll /= 360*Math.PI*2;
    var rollOver2 = roll * 0.5;
    var sinRollOver2 = Math.sin(rollOver2);
    var cosRollOver2 = Math.cos(rollOver2);
    var pitchOver2 = pitch * 0.5;
    var sinPitchOver2 = Math.sin(pitchOver2);
    var cosPitchOver2 = Math.cos(pitchOver2);
    var yawOver2 = yaw * 0.5;
    var sinYawOver2 = Math.sin(yawOver2);
    var cosYawOver2 = Math.cos(yawOver2);
    var result = new Quaternion();	
    result.w = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2;
    result.x = cosYawOver2 * sinPitchOver2 * cosRollOver2 + sinYawOver2 * cosPitchOver2 * sinRollOver2;
    result.y = sinYawOver2 * cosPitchOver2 * cosRollOver2 - cosYawOver2 * sinPitchOver2 * sinRollOver2;
    result.z = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2;

    return result;
}


function Vector3(x,y,z) {
	this.x = x
	this.y = y
	this.z = z
}

Vector3.prototype.add = function (v2) {
    var v = new Vector3(this.x + v2.x,
                       this.y + v2.y,
                       this.z + v2.z);
    return v;
}

Vector3.prototype.dot = function (v2) {
	// DOT PRODUCT OF TWO VECTORS
	// THIS and V2
	// RETURNS A NUMBER
	// 0 when they are 90 degrees on each other.
	// equiv to this.length() * v2.length() * Math.cos(ANGLE)
	return (this.x*v2.x) + (this.y*v2.y) + (this.z*v2.z)
}

Vector3.prototype.angle = function (v2) {
	// DOT PRODUCT OF TWO VECTORS
	// THIS and V2
	// RETURNS A NUMBER
	// 0 when they are 90 degrees on each other.
	// equiv to this.length() * v2.length() * Math.cos(ANGLE)
	//return this.x*v2.x + this.y*v2.y + this.z*v2.z
	var a = new Vector3(this.x,this.y,this.z)
	a.normalize();
	var b = new Vector3(v2.x,v2.y,v2.z)
	b.normalize();

	return Math.acos(a.dot(b))

	/*return Math.acos(this.dot(v2) / (this.length() * v2.length()));*/
}

Vector3.prototype.cross = function (v2) {
	//TODO
	var x = this.y*v2.z - this.z*v2.y
	var y = this.z*v2.x - this.x*v2.z
	var z = this.x*v2.y - this.y*v2.x
	////
    var v = new Vector3(x,y,z);
    return v;
}

Vector3.prototype.equal = function (v2) {
    return this.x == v2.x && this.y == v2.y && this.z == v2.z;
}

Vector3.prototype.length = function () {
	return Math.sqrt(this.x*this.x + this.y*this.y + this.z*this.z)
}

Vector3.prototype.normalize = function () {
	//turns vector into a unit length vector ( sets the magnitude to 1)
	var magnitude = this.length();
	this.x /= magnitude
	this.y /= magnitude
	this.z /= magnitude
}

Vector3.prototype.rotate = function(axis,theta) {
	//	Vector3.prototype.rotate = function(Vector3,float<=Math.PI) {
	
	//	rotates this vector around vector axis  by amount theta

	var q = new Quaternion();
	q.w = Math.cos(theta/2)
	q.x = axis.x * Math.sin(theta/2)
	q.y = axis.y * Math.sin(theta/2)
	q.z = axis.z * Math.sin(theta/2)

	var w = new Quaternion();
	w.w = 0.0
	w.x = this.x;
	w.y = this.y;
	w.z = this.z;

	//To invert, -xyz and w stays the same
	var qInv = new Quaternion();
	qInv.x = - q.x 
	qInv.y = - q.y 
	qInv.z = - q.z
	qInv.w = q.w

	var vRot = q.multiply(w).multiply(qInv)

	return new Vector3(vRot.x,vRot.y,vRot.z);
	//console.log(vRot)	
}

///quaternions

var v = new Vector3(0,0,-5)
var theta = Math.PI/2.0
var axis = new Vector3(1,0,0)
var q = new Quaternion();
q.w = Math.cos(theta/2)
q.x = axis.x * Math.sin(theta/2)
q.y = axis.y * Math.sin(theta/2)
q.z = axis.z * Math.sin(theta/2)

var w = new Quaternion();
w.w = 0.0
w.x = v.x;
w.y = v.y;
w.z = v.z;

//To invert, -xyz and w stays the same
var qInv = new Quaternion();
qInv.x = - q.x 
qInv.y = - q.y 
qInv.z = - q.z
qInv.w = q.w

var vRot = q.multiply(w).multiply(qInv)
console.log(vRot)

/*
function length() {
	sqrt(x*x + y*y + z*z + w*w)
}

function normalise() {
	q.x /= q.length
	q.y /= q.length
	q.z /= q.length
	q.w /= q.length
}

function create(angle) {
	q.w = Math.cos(angle/2)
	q.x = axisx * sin(angle/2)
	q.y = axisy * sin(angle/2)
	q.z = axisz * sin(angle/2)
}

function multiply(q2) {
	Let Q1 and Q2 be two quaternions, which are defined, respectively, as (w1, x1, y1, z1) and (w2, x2, y2, z2).
	(Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
	(Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
	(Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
	(Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2
}

function inverse(q) {
	//returns 1/q
	q.x *= -1;
	q.y *= -1;
	q.z *= -1;
}


////////////

V = Q*W*(1/Q)

Make a vector W, turn it into a Pure quaternion, multiply as above.
You get Quaternion V. Discard the V.w component. You have rotated W by quaternion.
V.x
V.y
V.z 

//PURE QUATERNION FROM VECTOR
W = {
	x: x,
	y: y,
	z: z,
	w: 0
}

//slerp = Spherical Linear intERPolation
//smooth rotation/intERPolation
*/