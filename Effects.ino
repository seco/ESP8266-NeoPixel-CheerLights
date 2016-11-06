void cycleLed(uint8_t wait) {
    for(int i=0; i<strip.numPixels(); i++) {
      strip.clear();
      
      for(int j=0; j<strip.numPixels(); j++) {
        if (i == j) {
          strip.setPixelColor(i, strip.Color(brightness, 0, 0));
        }
      }
      strip.show();
      delay(wait);
    }
    strip.clear();
    strip.show();
}

void colorWipe(uint32_t newColor, uint8_t wait) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, newColor);
        strip.show();
        delay(wait);
    }
}

int red(uint32_t color) {
  return (color >> 16) & 0xFF;
}

int green(uint32_t color) {
  return (color >> 8) & 0xFF;
}

int blue(uint32_t color) {
  return color & 0xFF;
}

void printColor(uint32_t color) {
  Serial.print(red(color));
  Serial.print(", ");
  Serial.print(green(color));
  Serial.print(", ");
  Serial.println(blue(color));
}

void fadeUpdate(uint32_t color1, uint32_t color2, uint8_t wait) {
  if (color1 == color2) {
    return;
  }
  
  double differenceRed = red(color2) - red(color1);
  double differenceGreen = green(color2) - green(color1);
  double differenceBlue = blue(color2) - blue(color1);
  
  double totalSteps = absoluteMax(differenceRed, differenceGreen, differenceBlue);
  
  for(int i = 0; i <= totalSteps; i++) {
    double intermediateRed = red(color1) + ((differenceRed / totalSteps) * i);
    double intermediateGreen = green(color1) + ((differenceGreen / totalSteps) * i);
    double intermediateBlue = blue(color1) + ((differenceBlue / totalSteps) * i);

    uint32_t intermediateColor = strip.Color((int)intermediateRed, (int)intermediateGreen, (int)intermediateBlue);

    printColor(intermediateColor);

    for(int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, intermediateColor);
    }

    strip.show();
    delay(wait);
  }
}

int absoluteMax(int r, int g, int b) {
  if (abs(r) > abs(g) && abs(r) > abs(b)) {
    return abs(r);
  } else if (abs(g) > abs(r) && abs(g) > abs(b)) {
    return abs(g);
  } else {
    return abs(b);
  }
}

