float Vector_Dot_Product(float vector1[3],float vector2[3])
{
  float op=0;
  
  for(int c=0; c<3; c++)
  {
  op+=vector1[c]*vector2[c];
  }
  
  return op; 
}

//Computes the cross product of two vectors
void Vector_Cross_Product(float vectorOut[3], float v1[3],float v2[3])
{
  vectorOut[0]= (v1[1]*v2[2]) - (v1[2]*v2[1]);
  vectorOut[1]= (v1[2]*v2[0]) - (v1[0]*v2[2]);
  vectorOut[2]= (v1[0]*v2[1]) - (v1[1]*v2[0]);
}

float VectorAngleTo(float a[3],float b[3]) {
  //returns the Angle between two vectors. PI = 180degrees.
  float aa[3] = {a[0],a[1],a[2]};
  Vector_normalize(aa);
  float bb[3] = {b[0],b[1],b[2]};
  Vector_normalize(bb);
  
  float dot = Vector_Dot_Product(aa,bb);
  return acos(dot);
}

//Multiply the vector by a scalar. 
void Vector_Scale(float vectorOut[3],float vectorIn[3], float scale2)
{
  for(int c=0; c<3; c++)
  {
   vectorOut[c]=vectorIn[c]*scale2; 
  }
}

void Vector_Add(float vectorOut[3],float vectorIn1[3], float vectorIn2[3])
{
  for(int c=0; c<3; c++)
  {
     vectorOut[c]=vectorIn1[c]+vectorIn2[c];
  }
}

float Vector_length(float v[3]) {

  return sqrt((v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]));
}

void Vector_normalize(float vectorOut[3]) {
  float mag = Vector_length(vectorOut);
  if (mag == 0) { mag = 0.0000001; }
  vectorOut[0] /= mag;
  vectorOut[1] /= mag;
  vectorOut[2] /= mag;
}

void VectorApplyAxisAngle(float vectorIn[3], float axis[3], float angle, float vectorOut[3]) {

 
  float qx = axis[0] * sin(angle/2);
  float qy = axis[1] * sin(angle/2);
  float qz = axis[2] * sin(angle/2);
  float qw = cos(angle/2);  
  float q[4] = {qx, qy, qz, qw};
  
  float wx = vectorIn[0];
  float wy = vectorIn[1];
  float wz = vectorIn[2];
  float ww = 0.0;
  float w[4] = {wx, wy, wz, ww};
  
  float qInvx = -qx;
  float qInvy = -qy;
  float qInvz = -qz;  
  float qInvw = qw;
  float qInv[4] = {-qx, -qy, -qz, qw};
  
  float vRot[4];
  QuaternionMultiply(vRot, q, w);
  float vRotb[4];
  QuaternionMultiply(vRotb, vRot, qInv);  
  
  vectorOut[0] = vRotb[0];
  vectorOut[1] = vRotb[1];
  vectorOut[2] = vRotb[2];  
}

void QuaternionMultiply(float outq[4], float q1[4],float q2[4]) { //xyzw
  outq[3] = q1[3]*q2[3] - q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2];
  outq[0] = q1[3]*q2[0] + q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1];
  outq[1] = q1[3]*q2[1] - q1[0]*q2[2] + q1[1]*q2[3] + q1[2]*q2[0];
  outq[2] = q1[3]*q2[2] + q1[0]*q2[1] - q1[1]*q2[0] + q1[2]*q2[3];
} 


