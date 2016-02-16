class Room {
  public:
    /* Constructor. */
    Room(int &number, String &name);
    
    /* Getters */
    float GetTemp();
    String GetName();
    byte GetSensor();
    
    /* Setters */
    void SetTemp(float &input_temp);
    void SetSensor(byte &sensor);
  private:
    byte number;
    byte sensor;
    String name;
    float target_temp;
    float current_temp;
}

Room::Room(int &number, String &name) {
  this->number = number;
  this->name = name;
  this->target_temp = 0.0;
  this->current_temp = 0.0;
  this->sensor = 0;
}

String Room::GetName() {
  return (this->name);
}

void Room::SetTemp(float &input_temp) {
  this->target_temp = input_temp;
}

float Room::GetTemp() {
  
  return (this->current_temp);
}

void Room::SetSensor(byte &sensor) {
  HTU21D sensor;
  this->sensor = sensor;
}

byte Room::GetSensor() {
  return (this->sensor);
}
