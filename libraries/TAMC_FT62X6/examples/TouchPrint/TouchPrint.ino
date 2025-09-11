#include "FT62X6.h"

FT62X6 tp = FT62X6();

void setup() {
  tp.begin();
  tp.setRotation(ROTATION_NORMAL);
  Serial.begin(115200);
  Serial.println("FT62X6 Example: Ready");
}

void loop() {
  tp.read();
  if (tp.isTouched){
    switch (tp.gesture){
      case MOVE_UP: Serial.println("Gesture: MOVE_UP");break;
      case MOVE_RIGHT: Serial.println("Gesture: MOVE_RIGHT");break;
      case MOVE_DOWN: Serial.println("Gesture: MOVE_DOWN");break;
      case MOVE_LEFT: Serial.println("Gesture: MOVE_UP");break;
      case ZOOM_IN: Serial.println("Gesture: ZOOM_IN");break;
      case ZOOM_OUT: Serial.println("Gesture: ZOOM_OUT");break;
      case NO_GESTURE:
        Serial.println("Gesture: NO_GESTURE");
        Serial.println("Touch 1:");
        Serial.print("  x: ");Serial.print(tp.points[0].x);
        Serial.print("  y: ");Serial.print(tp.points[0].y);
        Serial.print("  weight: ");Serial.print(tp.points[0].weight);
        Serial.print("  area: ");Serial.print(tp.points[0].area);
        Serial.println(' ');
        if (tp.touches == 2){
          Serial.println("Touch 2:");
          Serial.print("  x: ");Serial.print(tp.points[1].x);
          Serial.print("  y: ");Serial.print(tp.points[1].y);
          Serial.print("  weight: ");Serial.print(tp.points[1].weight);
          Serial.print("  area: ");Serial.print(tp.points[1].area);
          Serial.println(' ');
        }
        break;
      default: Serial.print("Gesture Error: ");Serial.println(tp.gesture);break;
    }
  }
}