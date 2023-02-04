int initial_time = 0;
bool digitalWrite_at_fixed_interval(int PIN_OUT, int millisecond_interval){

  // by default, this function will return false
  bool cycle_finished = false;

  // track how much time has passed
  int current_time = millis();
  int diff = current_time - initial_time;

  // once [2*interval] time passes, reset timer
  if(diff <= millisecond_interval){
    digitalWrite(PIN_OUT, 1);
  }else{
    if(diff <= 2*millisecond_interval){
      digitalWrite(PIN_OUT, 0);
    }else{
      digitalWrite(PIN_OUT, 0);
      initial_time = millis();
      cycle_finished = true;
    }
  }
  
  return(cycle_finished);
}
