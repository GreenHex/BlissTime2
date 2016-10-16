#include <pebble.h>
#include "global.h"
#include "config.h"
#include "clock.h"

#define NUM_INT_MESSAGE_KEYS 11
#define NUM_BOOL_MESSAGE_KEYS 2

static int32_t stringToInt( char *str );

void handle_config_message( DictionaryIterator *iterator ) {
  if ( !iterator ) return;

  // this is all specific to pebble/clay, which always returns "data" for integers
  // could be const arrays..., but we don't want to lose automatic key assignment...
  // in all-caps, pretending to be constant values. This should be outside the 
  // function, but since these are variables, tough luck.
  uint32_t INT_MESSAGE_KEYS[ NUM_INT_MESSAGE_KEYS ] = {
    // Status 
    MESSAGE_KEY_STATUS_DISPLAY_TYPE,
    MESSAGE_KEY_STATUS_UPDATE_INTERVAL,
    MESSAGE_KEY_STATUS_UPDATE_START_TIME,
    MESSAGE_KEY_STATUS_UPDATE_END_TIME,
    // Clock
    MESSAGE_KEY_CLOCK_TYPE_DIGITAL_OR_ANALOG,
    MESSAGE_KEY_DIGITAL_CLOCK_TYPE_12_OR_24,
    MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS,
    // Chime
    MESSAGE_KEY_CHIME_INTERVAL,
    MESSAGE_KEY_CHIME_START_TIME,
    MESSAGE_KEY_CHIME_END_TIME,
    MESSAGE_KEY_CHIME_OFFSET
  };

  uint32_t BOOL_MESSAGE_KEYS[ NUM_BOOL_MESSAGE_KEYS ] = {
    MESSAGE_KEY_STATUS_UPDATE_ON_DAYS,
    MESSAGE_KEY_CHIME_ON_DAYS
  };

  Tuple *p_tuple = 0;

  for ( int i = 0 ; i < NUM_INT_MESSAGE_KEYS; i++ ) {
    if ( ( p_tuple = dict_find( iterator, INT_MESSAGE_KEYS[ i ] ) ) ) {
      persist_write_int( INT_MESSAGE_KEYS[ i ], stringToInt( (char*) p_tuple->value->data ) );
    }
  }

  for ( int i = 0 ; i < NUM_BOOL_MESSAGE_KEYS; i++ ) {
    for ( int j = 0 ; j < 7 ; j++ ) {
      if ( ( p_tuple = dict_find( iterator, BOOL_MESSAGE_KEYS[i] + j ) ) ) {
        persist_write_bool( BOOL_MESSAGE_KEYS[i] + j, ( ( p_tuple->value->uint8 == 't' ) || ( p_tuple->value->uint8 == 'T' ) ) );
      }
    }
  }
  draw_clock();
}

static int32_t stringToInt(char *str){
  int i=0;
  int32_t sum=0;
  while(str[i]!='\0'){
    if(str[i]< 48 || str[i] > 57){
      if (DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "config.c: stringToInt(): Unable to convert it into integer.");
      return 0;
    }
    else{
      sum = sum*10 + (str[i] - 48);
      i++;
    }
  }
  return sum;
}
