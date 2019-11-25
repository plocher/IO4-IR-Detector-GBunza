
#define DET1  2
#define DET2  3
#define DET3  4
#define DET4  5
#define OUT1  6
#define OUT2  7
#define OUT3  8
#define OUT4  9
#define LED1  13
#define LED2  12
#define LED3  11
#define LED4  10

#define HYSTERESIS (long)2000    // in mS units, 2000 = 2.0 seconds

#define OCCUPIED LOW
#define EMPTY    ~OCCUPIED


#define smillis() ((long)millis())

class Detector {
public:
    Detector(int number, int det, int out, int led) {
      init(number, det, out, led);
    }
    
    Detector(void) {
        num       = -1;
        detPin    = -1;
        outPin    = -1;
        ledPin    = -1;
        delaytime = smillis();
        detected  = false;
    }
    
    void init(int number, int det, int out, int led) {
        num       = number;  // 1..4
        detPin    = det;
        outPin    = out;
        ledPin    = led;
        detected  = false;   // default to unoccupied...
        delaytime = smillis();  // expired...
        pinMode(ledPin,     OUTPUT);
        pinMode(outPin,     OUTPUT);
        pinMode(detPin,     INPUT);
        digitalWrite(outPin, detected ? OCCUPIED : EMPTY); 
        digitalWrite(ledPin, detected ? LOW : HIGH); 
    }
    
    boolean after(long timeout) {
        return smillis()-timeout>0;
    }

    boolean check(void) {
        if (num == -1) return false;
      
        int r1 =  digitalRead(detPin);
        if (r1 == HIGH) {
            detected = true;
            delaytime = smillis() + HYSTERESIS;  // expiration time is pushed out every time detection is seen
        }
        // hysteresis expired without further detection events
        if (after(delaytime)) {
            detected = false;
        }
        // report occupancy...
        digitalWrite(outPin, detected ? OCCUPIED : EMPTY); 
        digitalWrite(ledPin, detected ? LOW : HIGH); 
        return detected;
    }
    
private:  
    int      num;
    long     delaytime;
    boolean  detected;
    int      detPin;
    int      outPin;
    int      ledPin;
};


Detector circuit0(0, DET1, OUT1, LED1);
Detector circuit1(1, DET2, OUT2, LED2);
Detector circuit2(2, DET3, OUT3, LED3);
Detector circuit3(3, DET4, OUT4, LED4);

void setup() {
}

void loop() {
    circuit0.check();
    circuit1.check();
    circuit2.check();
    circuit3.check();
}
