void addRoom(room* roomList, String room_name, byte sensor_address, byte heater_address) {
  room* a_room = new room;
  a_room->room_name = room_name;
  a_room->sensor_address = sensor_address;
  a_room->heater_address = heater_address;
  a_room->sensor_reading = 0.0;
  a_room->heater_state = false;
  a_room->target_temperature = 0.0;
  a_room->p_next_room = roomList;
}