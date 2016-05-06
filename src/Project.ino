const int resolution = 2;    //resolution depends on mechanical properties
const int speedRPS = 3;
const int width = 10;

const int stepPin = 9;
const int dirPin =  8;
const int stepPin2 = 11;
const int dirPin2 =  10;

const int stepsInFullRound = 200;

int currentPos, goTo;
int coordinates[] = {1,2,3,4};

boolean endOfArray;

void setup() {
  
  Serial.begin(9600);

  pinMode(stepPin, OUTPUT);      
  pinMode(dirPin, OUTPUT);
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);  

  pinMode(stepPin2, OUTPUT);      
  pinMode(dirPin2, OUTPUT);
  digitalWrite(stepPin2, LOW);
  digitalWrite(dirPin2, LOW);
  
  currentPos = 1;
  endOfArray = true;
}

void moveIt(int destination){

    Serial.println("Inside moveIt");

    int destinationX, destinationY, currentX, currentY;
    
    currentY = currentPos/width * resolution;
    currentX = currentPos%width * resolution;
    destinationY = destination/width * resolution;
    destinationX = destination%width * resolution;

    while (currentX != destinationX || currentY != destinationY) {

      //check if neighbouring cell

      if (currentY != destinationY) {
        if (currentY > destinationY) {
          
          //move Y motor -1
          run(false, speedRPS, 1);
                  
          currentY-=1;
          Serial.println("Moving down 1 step");
        }
        else {
          //move Y motor +1  
          run(true, speedRPS, 1);
                
          currentY+=1;
          Serial.println("Moving up 1 step");
        }  
      }
      if (currentX != destinationX) {
        if (currentX > destinationX) {
          
          //move X motor -1       
          run2(false, speedRPS, 1);
           
          currentX -= 1;
          Serial.println("Moving left 1 step");
        }
        else {
          //move X motor +1  
          run2(true, speedRPS, 1);
                
          currentX += 1;
          Serial.println("Moving right 1 step");
        }  
      }
      currentPos = (currentY * width + currentX) / resolution;
      Serial.print("Current position: ");
      Serial.println(currentPos);
    }
}

// Runs the motor according to a chosen direction, speed (rounds per seconds) and the number of steps
void run(boolean runForward, double speedRPS, int stepCount) {
  digitalWrite(dirPin, runForward);
  for (int i = 0; i < stepCount; i++) {
    digitalWrite(stepPin, HIGH);
    holdHalfCylce(speedRPS);
    digitalWrite(stepPin, LOW);
    holdHalfCylce(speedRPS);
  }
}

void run2(boolean runForward, double speedRPS, int stepCount) {
  digitalWrite(dirPin2, runForward);
  for (int i = 0; i < stepCount; i++) {
    digitalWrite(stepPin2, HIGH);
    holdHalfCylce(speedRPS);
    digitalWrite(stepPin2, LOW);
    holdHalfCylce(speedRPS);
  }
}

// A delay function used in the run()-method
void holdHalfCylce(double speedRPS) {
  long holdTime_us = (long)(1.0 / (double) stepsInFullRound / speedRPS / 2.0 * 1E6);
  int overflowCount = holdTime_us / 65535;
  for (int i = 0; i < overflowCount; i++) {
    delayMicroseconds(65535);
  }
  delayMicroseconds((unsigned int) holdTime_us);
}

void loop() { 
  delay(1000);
  int lengthOfArray = sizeof(coordinates)/2;
  if (endOfArray){
    for (int i = 0; i < lengthOfArray; i++) {
      goTo = coordinates[i];
      if (goTo <= 0 || goTo > width*width)
        Serial.println("Invalid coordinate");
      else if (goTo == currentPos) {
        Serial.println("I am staying");
      }
      else {
        Serial.print("Go to position: ");
        Serial.println(goTo);      
        moveIt(goTo);
      } 
    }
    endOfArray = false;
  }  
}
