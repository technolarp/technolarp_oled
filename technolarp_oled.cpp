#include "technolarp_oled.h"

// init oled display 128*64
Adafruit_SSD1306 display_oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

M_oled::M_oled()
{
	animationActive=false;
	oledStatus=true;
	iterations = 0;
	
	interval = 100;
	previousMillis = 0;
	
	animActuelle = ANIM_NONE;
}

void M_oled::beginOled()
{
  // OLED
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display_oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // Address 0x3C for 128x32
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display_oled.clearDisplay();

  // texte en blanc
  display_oled.setTextColor(WHITE);
}


void M_oled::showDecimal()
{
	
}

void M_oled::displayText(const char * texteAAfficher, int taillePolice, bool videEcran, bool changementEcran, bool centered, bool crlf)
{
  displayText(texteAAfficher, taillePolice, videEcran, changementEcran, centered, crlf, false);
}

void M_oled::displayText(const char * texteAAfficher, int taillePolice, bool videEcran, bool changementEcran, bool centered, bool crlf, bool inverted)
{
  if (inverted)
  {
	  display_oled.setTextColor(SSD1306_BLACK, WHITE);
  }
  else
  {
	  display_oled.setTextColor(WHITE, SSD1306_BLACK);
  }
  
  if (videEcran)
  {
    display_oled.clearDisplay();
    display_oled.setCursor(0, 0);
  }

  display_oled.setTextSize(taillePolice);
  
  if (centered)
  {
    int16_t  x1, y1;
    uint16_t w, h;
    
    display_oled.getTextBounds(texteAAfficher, 0, 0, &x1, &y1, &w, &h);
  
    int newX = (SCREEN_WIDTH - w)/2;
    int newY = (SCREEN_HEIGHT - h)/2;
    
    display_oled.setCursor(newX, newY);
  }
  display_oled.print(texteAAfficher);
  
  
  if (crlf)
  {
    display_oled.println();
  }

  if(changementEcran)
  {
    display_oled.display();
  }
}


void M_oled::displayRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h)
{
	display_oled.drawRect(x0, y0, w, h, WHITE);
}

void M_oled::displayFillRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h)
{
	display_oled.fillRect(x0, y0, w, h, WHITE);
}

void M_oled::display()
{
	display_oled.display();
}

void M_oled::clearDisplay()
{
	display_oled.clearDisplay();
    display_oled.setCursor(0, 0);
}

void M_oled::displayCircle(uint16_t x0, uint16_t y0, uint16_t r)
{
	display_oled.drawCircle(x0, y0, r, WHITE);
}

void M_oled::displayFillCircle(uint16_t x0, uint16_t y0, uint16_t r)
{
	display_oled.fillCircle(x0, y0, r, WHITE);
}

void M_oled::updateAnimation()
{
	if (animationActive)
	{
		if(millis() - previousMillis > interval)
		{
			previousMillis = millis();
			oledStatus = !oledStatus;
			
			switchAnim(animActuelle);
			
			if (iterations>0)
			{
				iterations-=1;
			}
			
			if (iterations==0)
			{
				animationActive = false;
				switchAnimEnd(animActuelle);
				animActuelle = ANIM_NONE;
			}
		}
	}
}

bool M_oled::isAnimActive()
{
	if (animActuelle == ANIM_NONE)
	{
		return(false);
	}
	else
	{
		return(true);
	}
}

void M_oled::switchAnim(uint8_t anim)
{
	switch (anim) 
    {
      case ANIM_BLINK:
        animationBlink();
      break;
      break;
	  
      default:
        // nothing to do
      break;
    }
}

void M_oled::switchAnimEnd(uint8_t anim)
{
	switch (anim) 
    {
      case ANIM_BLINK:
        animationBlinkEnd();
      break;
	  
      default:
        // nothing to do
      break;
    }
}

void M_oled::animationBlinkEnd()
{
	display_oled.clearDisplay();
}

void M_oled::animationBlink()
{
	display_oled.clearDisplay();
	if (oledStatus)
	{
		displayFillCircle(63, 31, 15);
	}
	else
	{
		displayCircle(63, 31, 15);
	}
	display_oled.display();
}

void M_oled::animationBlink01Start(uint16_t intervalToSet, uint16_t iterationToSet)
{
	animActuelle = ANIM_BLINK;
	animationActive=true;
	oledStatus=true;
	
	iterations = iterationToSet;
	interval = intervalToSet;
	
	previousMillis = millis();
}

void M_oled::animationBlink02Start(uint16_t intervalToSet, uint16_t dureeToSet)
{
	animActuelle = ANIM_BLINK;
	animationActive=true;
	oledStatus=true;
	
	iterations = dureeToSet/intervalToSet;
	interval = intervalToSet;
	
	previousMillis = millis();
}

void M_oled::animationDepart()
{
	display_oled.clearDisplay();
	//displayText(String texteAAfficher, int taillePolice, bool videEcran, bool changementEcran, bool centered, bool crlf, bool inverted)
	displayText("INITIALISATION", 1, true, false, false, false, false);
	displayRect(0, 20, SCREEN_WIDTH, 20);
	
	for (uint8_t i=0;i<SCREEN_WIDTH;i++)
	{
		displayFillRect(0, 20, i, 20);
		display_oled.display();
		delay(5);
	}
}
