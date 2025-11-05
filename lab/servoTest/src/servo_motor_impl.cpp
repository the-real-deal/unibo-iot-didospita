#include "Arduino.h"
#include "servo_motor_impl.h"

ServoMotorImpl::ServoMotorImpl(int pin){
  this->pin = pin;  
} 

void ServoMotorImpl::on(){
  motor.attach(pin);    
}

void ServoMotorImpl::setPosition(int angle){
  motor.write(angle);              
}

void ServoMotorImpl::off(){
  motor.detach();    
}
