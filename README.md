# arduino attiny85 mcp23017 library
## UNSTABLE / in development
mcp23017 library for attiny85.


example code:
```cpp
#include <Arduino.h>

#include <MCP23017.h>

MCP23017 mcp;

void setup()
{

  mcp.begin();
  for (int i = 0; i < 16; i++)
  {
    mcp.pinMode(i, MCP_OUTPUT);
  }
  pinMode(3, OUTPUT);

  mcp.pinMode(15, MCP_PULLUP);
}

void loop()
{

  mcp.digitalWrite(1, mcp.digitalRead(15));

  mcp.digitalWrite(8, LOW); // PORT B pin 0
  mcp.digitalWrite(0, HIGH); // PORT A pin 0
  
  delay(100);
  mcp.digitalWrite(8, HIGH); // PORT B pin 0
  mcp.digitalWrite(0, LOW); // PORT A pin 0
  digitalWrite(3, LOW);
  delay(100);
}
```