#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <PS2X_lib.h>
#include "eeb.h"
#include "configs.h"


Servo_Motor     VRC_Servo;
PS2X            VRC_PS2;
DCMotor         VRC_Motor;

int16_t pwm_left = 0, pwm_right = 0;
bool dir_left = 0, dir_right = 0;

int servo_1_angle = 0;
int servo_2_angle = 0;



void VRC_Control(){
  if(VRC_PS2.Button(PSB_CIRCLE)){
      Serial.println("Hinh TRON");
      VRC_Servo.Angle(50, RONTARY_SERVO);
  } 
  else  if(VRC_PS2.Button(PSB_TRIANGLE)){
    Serial.println("Hinh TAM GIAC");
    VRC_Servo.Angle(150, RONTARY_SERVO);
  } else VRC_Servo.Stop(RONTARY_SERVO);

  if(VRC_PS2.Button(PSB_CROSS)){
    // while(VRC_PS2.ButtonPressed(PSB_CIRCLE));
    Serial.println("Hinh PSB_CROSS");
      VRC_Servo.Angle(50, RONTARY_SERVO2);
      delay(20);
      VRC_Servo.Stop(RONTARY_SERVO2);
  } else if(VRC_PS2.Button(PSB_SQUARE)){
    // while(VRC_PS2.ButtonPressed(PSB_CIRCLE));
    Serial.println("Hinh PSB_SQUARE");
    VRC_Servo.Angle(150, RONTARY_SERVO2);
      delay(20);
      VRC_Servo.Stop(RONTARY_SERVO2);
  }  else VRC_Servo.Stop(RONTARY_SERVO2);

  // ********** Lấy bóng ***********
    if(VRC_PS2.Button(PSB_L1)){
      Serial.println("L1");
      VRC_Motor.Run(COLLECT_MOTOR, MAX_SPEED, GET_BALL);
    } else if(VRC_PS2.Button(PSB_L2)){
      Serial.println("R2");
      VRC_Motor.Run(COLLECT_MOTOR, MAX_SPEED, !GET_BALL);
    } else VRC_Motor.Stop(COLLECT_MOTOR);
  // ********** end dirrection mode ********************************//
 // ********** đẩy bóng vào chỗ bắn bóng ***********
    if(VRC_PS2.Button(PSB_PAD_UP)){
      // Serial.println("R1");
      // VRC_Servo.Angle(60,  HOLDER_SERVO);
      if( servo_1_angle < 180) servo_1_angle += 3;
    } 
    if(VRC_PS2.Button(PSB_PAD_DOWN)){
      // VRC_Servo.Angle(180, HOLDER_SERVO);
      if (servo_1_angle > 0) servo_1_angle -= 3;
    }
      VRC_Servo.Angle(servo_1_angle, HOLDER_SERVO);
      
    if(VRC_PS2.Button(PSB_PAD_LEFT)){
      // Serial.println("R1");
      // VRC_Servo.Angle(60,  HOLDER_SERVO);
      if( servo_2_angle < 180) servo_2_angle += 3;
    } 
    if(VRC_PS2.Button(PSB_PAD_RIGHT)){
      // VRC_Servo.Angle(180, HOLDER_SERVO);
      if (servo_2_angle > 0) servo_2_angle -= 3;
    }
      VRC_Servo.Angle(servo_2_angle, HOLDER_SERVO2);
  // ********** end dirrection mode ********************************//

  // ********** Bắn bóng ***********
    if(VRC_PS2.ButtonPressed(PSB_R1)){
      Serial.println("R1");
      VRC_Motor.Run(SHOOT_MOTOR, SHOOT_MOTOR_SPEED, GET_BALL);
    }
    if(VRC_PS2.ButtonPressed(PSB_R2)){
      Serial.println("R2");
      VRC_Motor.Stop(SHOOT_MOTOR);
    }
  // ********** end dirrection mode ********************************//


 
  // ********** dirrection Mode ***********
    if(VRC_PS2.ButtonPressed(PSB_L3)){
      Serial.println("L3");
    }
    if(VRC_PS2.ButtonPressed(PSB_R3)){
      Serial.println("R3");
    }
  // ********** end dirrection mode ********************************//

  // ********************************** CONTROL MODE ****************************************** //  
    // **************************** MOVING ROBOT ALGORITHM ********************// 
    int16_t val_RY, val_RX;
    // if(dirrection==1){
      val_RY = VRC_PS2.Analog(PSS_RY);
      val_RX = VRC_PS2.Analog(PSS_RX);
    // }
    // else{
    //   val_RY = VRC_PS2.Analog(PSS_LY);
    //   val_RX = VRC_PS2.Analog(PSS_LX);
    // }
    
    // loc nhieu
    if(val_RY>=NOISE_J_UP || val_RY<=NOISE_J_DOWN){
      val_RY = map(val_RY,0,255,MAX_PWM,-MAX_PWM);
    }
    else val_RY = 0;
    if(val_RX>=NOISE_J_UP || val_RX<=NOISE_J_DOWN){
      val_RX = map(val_RX,0,255,MAX_PWM,-MAX_PWM)*ROTATE_SPEED_SCALE;
    }
    else val_RX=0;

    // tinh toan

    if(val_RY>=0){
        pwm_left = val_RY - val_RX;
        pwm_right = val_RY + val_RX;
    }

    else{
       pwm_left = val_RY + val_RX;
       pwm_right = val_RY - val_RX; 
    }


    if(abs(pwm_left)<=MIN_PWM) pwm_left = 0;
    if(abs(pwm_right)<=MIN_PWM) pwm_right = 0;

    if(pwm_right>MAX_PWM ) pwm_right = MAX_PWM ;
    if(pwm_right<-MAX_PWM ) pwm_right = -MAX_PWM ;

    if(pwm_left>MAX_PWM ) pwm_left = MAX_PWM ;
    if(pwm_left<-MAX_PWM ) pwm_left = -MAX_PWM ;

    if(pwm_left >=0) dir_left =0;
    else {
      dir_left =1; 
      pwm_left = -pwm_left;
    }
    if(pwm_right >=0) dir_right =0;
    else {
      dir_right =1; 
      pwm_right = -pwm_right;
    }
  // ********************** END AGORITHM **************** //


}

void setup() {

  Serial.begin(9600);
  //config ps2:
  int err = -1;

  for(int i=0; i<10; i++){
    delay(100);
    err = VRC_PS2.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    Serial.print(".");
    if(!err){
      Serial.println("Sucsessfully Connect PS2 Controller!");
      break;
    }
  }
  if(err) Serial.println("Failed Connect PS2 Controller!");

  VRC_Motor.Init();
  VRC_Servo.Init();

  Serial.println("Done Setup");
} 

void loop(){
      VRC_PS2.read_gamepad(false, false);

      VRC_Control();
      VRC_Motor.Run(LEFT_MOTOR,pwm_left,dir_left);
      VRC_Motor.Run(RIGHT_MOTOR,pwm_right,dir_right);
      delay(10);
}