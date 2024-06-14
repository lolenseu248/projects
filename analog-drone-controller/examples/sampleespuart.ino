 mavlink_message_t msg;
    mavlink_status_t status;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    while (Serial2.available()>0){
      uint8_t c=Serial2.read();
      if (mavlink_parse_char(MAVLINK_COMM_0,c,&msg,&status)){
        uint16_t len = mavlink_msg_to_send_buffer(buf,&msg);
        Serial.write(buf,len);
      }
    }
    while (Serial.available()>0) {
      uint8_t c=Serial.read();
      if (mavlink_parse_char(MAVLINK_COMM_0,c,&msg,&status)){
        uint16_t len=mavlink_msg_to_send_buffer(buf,&msg);
        Serial2.write(buf,len);
      }
    }
    mavlink_msg_heartbeat_pack(1,MAV_COMP_ID_AUTOPILOT1,&msg,MAV_TYPE_QUADROTOR,MAV_AUTOPILOT_GENERIC,MAV_MODE_FLAG_MANUAL_INPUT_ENABLED,0,MAV_STATE_STANDBY);
    uint16_t len=mavlink_msg_to_send_buffer(buf,&msg);
    Serial2.write(buf,len);