// current_rule: 0 is silence and 1 is sound
// s: number of events in a cycle (smaller number)
// b: number of beats in a cycle (larger number)
// o: offset for resultant pattern
void update_euclid_rule(int current_rule[32], int s, int b, int o){

  int N_int = s; // number of sounds
  int N_0 = b - s; // number of silences
  int N_0_per_int = N_0 / s; // number of rests to allocate after EVERY beat
  int count_extra_rests = N_0 % N_int; // number of extra rests to allocate irregularly
  int count_no_extra_rests = N_int - count_extra_rests; // number of sounds WITHOUT extra rest

  int counter = 0 + o;
  for(int i = 0; i < N_int; i = i + 1){

    // accomodate offset, by going back to the beginning
    if(counter > b){
      counter = 1;
    }
    
    // Record the beat
    current_rule[counter] = 1; // add beat
    counter = counter + 1;

    // If there are rests after EVERY beat,
    //  add those rests now.
    if(N_0_per_int > 0){
      for(int j = 0; j < N_0_per_int; j = j + 1){
        current_rule[counter] = 0; // add rest
        counter = counter + 1;
      }
    }

    // If there are any extra beats that need to get added
    //  check whether to add them now.
    if(count_extra_rests > 0){
      // if beat is even and nothing should be done (i.e., no beat or rest)
      //  do nothing, but decrease the do-nothing counter
      if((i % 2) == 0 & count_no_extra_rests > 0){
        count_no_extra_rests = count_no_extra_rests - 1; // this is a do-nothing counter
      }else{
        // otherwise, add the extra rest then decrease the do-something counter
        current_rule[counter] = 0; // add rest
        counter = counter + 1;
        count_extra_rests = count_extra_rests - 1; // this is a do-something counter
      }
    }
  }
}
