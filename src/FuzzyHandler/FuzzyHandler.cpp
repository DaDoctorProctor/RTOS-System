#include "FuzzyHandler.h"

// Constructor implementation
FuzzyHandler::FuzzyHandler() : ref(0), pos_act(0), ekT_ant(0.0), sum_ekT(0.0) {
    fuzzy = new Fuzzy();
}

// FreeRTOS task function
void FuzzyHandler::taskWrapper(void* pvParameters) {
    FuzzyHandler* instance = static_cast<FuzzyHandler*>(pvParameters);
    instance->taskFuzzy();
}

// Singleton instance getter
FuzzyHandler& FuzzyHandler::getInstance() {
    static FuzzyHandler instance; // Static instance ensures it's created once
    return instance;
}

// Task process function
void FuzzyHandler::taskFuzzy() {
    while(true) {
        handleFuzzy();
        vTaskDelay(10 / portTICK_PERIOD_MS); // 10ms delay
    }
}


void FuzzyHandler::begin(){

  // Input - encoder
  FuzzyInput * error = new FuzzyInput(1);
  
  FuzzySet * ENG= new FuzzySet(-2900.0, -2900.0, -2250.0,-1125.0);
  error->addFuzzySet(ENG);  

  FuzzySet * ENP= new FuzzySet(-2250.0, -1125.0, -1125.0,0.0);
  error->addFuzzySet(ENP);  

  FuzzySet * EC= new FuzzySet(-1125.0, -10.0, 10.0,1125.0);
  error->addFuzzySet(EC);  

  FuzzySet * EPP= new FuzzySet(0.0, 1125.0, 1125.0,2250.0);
  error->addFuzzySet(EPP);  

  FuzzySet * EPG= new FuzzySet(1125.0, 2250.0, 2900.0,2900.0);
  error->addFuzzySet(EPG);

  fuzzy->addFuzzyInput(error);

  // Output - pwm
  FuzzyOutput * PwM = new FuzzyOutput(1);

  FuzzySet *PwMNG= new FuzzySet(-100.0, -100.0, -50.0,-25.0);
  PwM->addFuzzySet(PwMNG);

  FuzzySet *PwMNP = new FuzzySet(-50.0,-25.0,-25.0,0.0);
  PwM->addFuzzySet(PwMNP);

  FuzzySet *PwMC = new FuzzySet(-25.0, -0.0,0.0,25.0);
  PwM->addFuzzySet(PwMC);
  
  FuzzySet *PwMPP = new FuzzySet(0.0,25.0, 25.0,50.0);
  PwM->addFuzzySet(PwMPP);

  FuzzySet *PwMPG = new FuzzySet(25.0,50.0, 100.0,100.0);
  PwM->addFuzzySet(PwMPG);

  fuzzy->addFuzzyOutput(PwM);
  

  // Rules - join with each other
  FuzzyRuleAntecedent *siENG = new FuzzyRuleAntecedent();
  siENG->joinSingle(ENG);
  FuzzyRuleConsequent *entoncesPwMNG = new FuzzyRuleConsequent();
  entoncesPwMNG->addOutput(PwMNG);
  FuzzyRule *Regla01 = new FuzzyRule(1,siENG ,entoncesPwMNG);
  fuzzy->addFuzzyRule(Regla01);


  FuzzyRuleAntecedent *siENP = new FuzzyRuleAntecedent();
  siENP->joinSingle(ENP);
  FuzzyRuleConsequent *entoncesPwMNP = new FuzzyRuleConsequent();
  entoncesPwMNP->addOutput(PwMNP);
  FuzzyRule *Regla02 = new FuzzyRule(2,siENP ,entoncesPwMNP);
  fuzzy->addFuzzyRule(Regla02);


   FuzzyRuleAntecedent *siEC = new FuzzyRuleAntecedent();
  siEC->joinSingle(EC);
  FuzzyRuleConsequent *entoncesPwMC = new FuzzyRuleConsequent();
  entoncesPwMC->addOutput(PwMC);
  FuzzyRule *Regla03 = new FuzzyRule(3,siEC ,entoncesPwMC);
  fuzzy->addFuzzyRule(Regla03);


  FuzzyRuleAntecedent *siEPP= new FuzzyRuleAntecedent();
  siEPP->joinSingle(EPP);
  FuzzyRuleConsequent *entoncesPwMPP = new FuzzyRuleConsequent();
  entoncesPwMPP->addOutput(PwMPP);
  FuzzyRule *Regla04 = new FuzzyRule(4,siEPP ,entoncesPwMPP);
  fuzzy->addFuzzyRule(Regla04);


  FuzzyRuleAntecedent *siEPG = new FuzzyRuleAntecedent();
  siEPG->joinSingle(EPG);
  FuzzyRuleConsequent *entoncesPwMPG = new FuzzyRuleConsequent();
  entoncesPwMPG->addOutput(PwMPG);
  FuzzyRule *Regla05 = new FuzzyRule(5,siEPG ,entoncesPwMPG);
  fuzzy->addFuzzyRule(Regla05);


}


void FuzzyHandler::handleFuzzy(){
  EncoderHandler& encoderHandler = EncoderHandler::getInstance();
  pos_act = encoderHandler.getPosition();
  int pos = pos_act;
  ref = getReference();

  //Error
  int ekT=pos-ref;
  fuzzy->setInput(1,ekT);
  fuzzy->fuzzify();
  float mkT=fuzzy->defuzzify(1);

  //Valor a enviar al motor
  float valor=fabs(mkT);
  float valpwm=map(valor, 0,100,190,255);

  if(valpwm>255)
    valpwm=255;
  if (ekT==0)
    valpwm=0;

  ModeSelector& modeSelector = ModeSelector::getInstance();
  if(modeSelector.getMode() == 2){ // check if the controller runs on auto or manual
    if(mkT<0){
        sentidoHor((byte)valpwm);
    }else{
        sentidoAntiHor((byte)valpwm);
    }

    Serial.print(pos);
    Serial.print(" ");
    Serial.print(ref);
    Serial.print(" ");
    Serial.print(ekT);
    Serial.print(" ");
    Serial.print(valpwm);
    Serial.println();
    vTaskDelay(10 / portTICK_PERIOD_MS); // 10ms delay

  }

  ekT_ant=ekT;

}


//El motor gira en el sentido de las manecillas del reloj
void FuzzyHandler::sentidoHor(byte vel){
    WheelHandler& wheelHandler = WheelHandler::getInstance();
    EncoderHandler& encoderHandler = EncoderHandler::getInstance();
    wheelHandler.setWheelDirection(false);
    if (encoderHandler.getPosition() >= (ref - fT) && encoderHandler.getPosition() <= (ref + fT)){
        wheelHandler.setMovement(0);
    } else {
        wheelHandler.setMovement(vel);
    }
    
}

void FuzzyHandler::sentidoAntiHor(byte vel){
    WheelHandler& wheelHandler = WheelHandler::getInstance();
    EncoderHandler& encoderHandler = EncoderHandler::getInstance();
    wheelHandler.setWheelDirection(true);

    if (encoderHandler.getPosition() >= (ref - fT) && encoderHandler.getPosition() <= (ref + fT)){
        wheelHandler.setMovement(0);
    } else {
        wheelHandler.setMovement(vel);
    }

}

void FuzzyHandler::freno(){
    WheelHandler& wheelHandler = WheelHandler::getInstance();
    wheelHandler.setMovement(0);
}




int FuzzyHandler::HorizontalInterpreter(int pixels_x){
    // Step 0 reverse signs
    //pixels_x = pixels_x; // Reverse the sign of the number
    if (pixels_x >= -4 && pixels_x <= 4) {
        return 0;
    }
    // applies for 23 cm distance
    // if the maximum 90 degress is 500 ref, 
    const int degree_90_ref = 400;
    const int pixel_quantity = 20;
    float ref = (pixels_x * degree_90_ref) / pixel_quantity;

    return ref;
}




// Store the incoming pixels from the camera
void FuzzyHandler::setPixelsX(int px) {
    pixelsX = px;
}

int FuzzyHandler::getPixelsX() const {
    return pixelsX;
}



int FuzzyHandler::getReference() const {
    return ref;
}

void FuzzyHandler::setReference(int newRef) {
    ref = newRef;
}
