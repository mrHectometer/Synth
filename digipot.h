const int FilterFreq1Pin = 10;
const int FilterRes1Pin = 9;
void digitalPotInit()
{
    pinMode (FilterFreq1Pin, OUTPUT);
    pinMode (FilterRes1Pin, OUTPUT);
}

void digitalPot1Write(int value) {
  // take the SS pin low to select the chip:
  digitalWriteFast(FilterFreq1Pin,LOW);
  //  send in the address and value via spi:
  //last 2 bits: pot select
  //other 1: write command
  SPI.transfer(0b00010011);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWriteFast(FilterFreq1Pin,HIGH); 
}
void digitalPotResWrite(int value) {
  // take the SS pin low to select the chip:
  digitalWriteFast(FilterRes1Pin,LOW);
  //  send in the address and value via spi:
  //last 2 bits: pot select
  //other 1: write command
  SPI.transfer(0b00010011);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWriteFast(FilterRes1Pin,HIGH); 
}
