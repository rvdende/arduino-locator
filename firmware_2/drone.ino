float copterForward[3] = {0.0,1.0,0.0};  
float copterUp[3] = {0.0,0.0,1.0};  
float copterRight[3] = {1.0,0.0,0.0}; 

float copterAccel[3] = {0,0,-1.0};        
float copterMagnet[3] = {1.0,1.0,1.0};  

bool magnetlocked = false;
float magnetLock[3] = {1.0,1.0,1.0}; 
float magnetLast[3] = {1.0,1.0,1.0}; 


float axisForward[3] = {0.0,1.0,0.0};  
float axisUp[3] = {0.0,0.0,1.0};  
float axisRight[3] = {1.0,0.0,0.0};  

float m_min[3] = {-707,-717,-461};
float m_max[3] = {-109,70,123};

long int oldtime = 0;

void droneUpdate(float ax,float ay,float az,float gx,float gy,float gz,float mx,float my,float mz) {
	axisForward[0] = 0; axisForward[1] = 1.0; axisForward[2] = 0.0;
	axisUp[0] = 0; axisUp[1] = 0.0; axisUp[2] = 1.0;
	Vector_Cross_Product(axisRight, axisForward, axisUp);  
        Vector_normalize(axisRight);

        //calc calibration limits of magnetometer gauss range
	m_min[0] = min(m_min[0], mx);
	m_min[1] = min(m_min[1], my);
	m_min[2] = min(m_min[2], mz);
	m_max[0] = max(m_max[0], mx);
	m_max[1] = max(m_max[1], my);
	m_max[2] = max(m_max[2], mz);


        //calc time
        long int deltatime = micros() - oldtime;        
        double deltatimes = double(deltatime) / 1000000.0;        
	oldtime = micros(); 
        
	if (deltatimes > 0.1) {  
	    Serial.println("Updating clock");
	} else {
            double rate = deltatimes/100.0/360.0*PI*2.0;  

//Apply gyro data
            //VectorApplyAxisAngle(float vectorIn[3], float axis[3], float angle, float vectorOut[3]) {
            VectorApplyAxisAngle(copterUp,copterForward,gy*rate,copterUp);
            VectorApplyAxisAngle(copterRight,copterForward,gy*rate,copterRight);  
            
            VectorApplyAxisAngle(copterUp,copterRight,-gx*rate,copterUp);
            VectorApplyAxisAngle(copterForward,copterRight,-gx*rate,copterForward);  
            
            VectorApplyAxisAngle(copterForward,copterUp,-gz*rate,copterForward);
            VectorApplyAxisAngle(copterRight,copterUp,-gz*rate,copterRight);  

//Load accelerometer vector (aligned to origin);                    
            copterAccel[0] = ax; copterAccel[1] = -ay;  copterAccel[2] = az;  
            Vector_normalize(copterAccel);

//Load magnetometer vector (aligned to origin);
    	    copterMagnet[0] = ((mx - m_min[0]) / (m_max[0] - m_min[0]) * 2) - 1.0;
            copterMagnet[1] = ((my - m_min[1]) / (m_max[1] - m_min[1]) * 2) - 1.0;
      	    copterMagnet[2] = ((mz - m_min[2]) / (m_max[2] - m_min[2]) * 2) - 1.0;
    	    copterMagnet[1] *= -1;
            
//Calculate copter difference to origin axis
            float realforward[3] = {0.0,1.0,0.0};  
            float forwarddiff = VectorAngleTo( copterForward, realforward );
            float forwardcross[3] = {0,0,0};
            //Vector_Cross_Product(float vectorOut[3], float v1[3],float v2[3])
            Vector_Cross_Product(forwardcross, realforward, copterForward);  
            Vector_normalize(forwardcross);
            //Apply to datavectors to correct for deviation from forward vector
            VectorApplyAxisAngle(axisForward, forwardcross,forwarddiff, axisForward);
            VectorApplyAxisAngle(axisUp, forwardcross,forwarddiff, axisUp);
            VectorApplyAxisAngle(axisRight, forwardcross,forwarddiff, axisRight);
            VectorApplyAxisAngle(copterAccel, forwardcross,forwarddiff, copterAccel);
            VectorApplyAxisAngle(copterMagnet, forwardcross,forwarddiff, copterMagnet);

  
            float rightdiff = VectorAngleTo( copterRight, axisRight );
            float rightcross[3] = {0,0,0};
            //Vector_Cross_Product(float vectorOut[3], float v1[3],float v2[3])
            Vector_Cross_Product(rightcross, axisRight, copterRight);  
            Vector_normalize(rightcross);
            //Apply to datavectors to correct for deviation from right vector
            VectorApplyAxisAngle(axisForward, rightcross,rightdiff, axisForward);
            VectorApplyAxisAngle(axisUp, rightcross,rightdiff, axisUp);
            VectorApplyAxisAngle(axisRight, rightcross,rightdiff, axisRight);
            VectorApplyAxisAngle(copterAccel, rightcross,rightdiff, copterAccel);
            VectorApplyAxisAngle(copterMagnet, rightcross,rightdiff, copterMagnet);

          /// CORRECT DRIFT USING ANGLE BETWEEN GRAVITY AND ACCELEROMETER VECTOR           
            float gravity[3] = {0,0,-1.0};
            float gravityerror = VectorAngleTo(copterAccel, gravity);
            float gravitycross[3] = {0,0,0};
            Vector_Cross_Product(gravitycross, copterAccel, gravity);  
            Vector_normalize(gravitycross);
            
            gravityerror *= 0.05; //SLOWS DOWN CORRECTION
            //Apply to datavectors to correct for deviation from down (gravity)
            VectorApplyAxisAngle(copterForward, gravitycross,gravityerror, copterForward);
            VectorApplyAxisAngle(copterUp, gravitycross,gravityerror, copterUp);
            VectorApplyAxisAngle(copterRight, gravitycross,gravityerror, copterRight);
            VectorApplyAxisAngle(copterAccel, gravitycross,gravityerror, copterAccel);
            VectorApplyAxisAngle(copterMagnet, gravitycross,gravityerror, copterMagnet);
            
            magnetLast[0] = copterMagnet[0];
            magnetLast[1] = copterMagnet[1];
            magnetLast[2] = copterMagnet[2];  
            
            if (magnetlocked) {
              
              float magnetcross[3] = {0,0,0};
              Vector_Cross_Product(magnetcross, copterMagnet, magnetLock);
              Vector_normalize(magnetcross);
              float magneterror = VectorAngleTo(copterMagnet, magnetLock);
              magneterror *= 0.03; //SLOWS DOWN CORRECTION
              VectorApplyAxisAngle(copterForward, magnetcross,magneterror, copterForward);
              VectorApplyAxisAngle(copterUp, magnetcross,magneterror, copterUp);
              VectorApplyAxisAngle(copterRight, magnetcross,magneterror, copterRight);
              VectorApplyAxisAngle(copterAccel, magnetcross,magneterror, copterAccel);
              VectorApplyAxisAngle(copterMagnet, magnetcross,magneterror, copterMagnet);
              
            } else {
              copterDoMagLock();
            }
            
     /*       
            //todo Magnet lock heading
            //Serial.println(gravityerror,DEC);
            */
            printvectors(); 
	}

}

void copterDoMagLock() {
  magnetlocked = true;
  magnetLock[0] = magnetLast[0];
  magnetLock[1] = magnetLast[1];
  magnetLock[2] = magnetLast[2];  
}

void printvectors() {
    Serial.print("{ \"fx\" : \"");
    Serial.print(copterForward[0]);
    Serial.print("\",");  
    Serial.print("\"fy\" : \"");
    Serial.print(copterForward[1]);  
    Serial.print("\",");  
    Serial.print("\"fz\" : \"");
    Serial.print(copterForward[2]);  
    Serial.print("\",");     
    Serial.print("\"ux\" : \"");
    Serial.print(copterUp[0]);  
    Serial.print("\",");      
    Serial.print("\"uy\" : \"");
    Serial.print(copterUp[1]);  
    Serial.print("\",");      
    Serial.print("\"uz\" : \"");
    Serial.print(copterUp[2]);  
    Serial.print("\",");      
    Serial.print("\"rx\" : \"");
    Serial.print(copterRight[0]);  
    Serial.print("\",");  
    Serial.print("\"ry\" : \"");
    Serial.print(copterRight[1]);  
    Serial.print("\",");  
    Serial.print("\"rz\" : \"");
    Serial.print(copterRight[2]);  
    Serial.print("\",");     
    Serial.print("\"afx\" : \"");
    Serial.print(axisForward[0]);
    Serial.print("\",");  
    Serial.print("\"afy\" : \"");
    Serial.print(axisForward[1]);  
    Serial.print("\",");  
    Serial.print("\"afz\" : \"");
    Serial.print(axisForward[2]);  
    Serial.print("\",");     
    Serial.print("\"aux\" : \"");
    Serial.print(copterAccel[0]);  
    Serial.print("\",");      
    Serial.print("\"auy\" : \"");
    Serial.print(copterAccel[1]);  
    Serial.print("\",");      
    Serial.print("\"auz\" : \"");
    Serial.print(copterAccel[2]);  
    Serial.print("\",");      
    Serial.print("\"arx\" : \"");
    Serial.print(copterMagnet[0]);  
    Serial.print("\",");  
    Serial.print("\"ary\" : \"");
    Serial.print(copterMagnet[1]);  
    Serial.print("\",");  
    Serial.print("\"arz\" : \"");
    Serial.print(copterMagnet[2]);      
    Serial.println("\"}");
}
