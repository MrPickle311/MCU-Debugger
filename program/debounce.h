#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "../drivers/core/timer.h"
#include "../drivers/core/bit_utils.h"

// Buttons connected to port D
#define BUTTON_PORT PORTD
#define BUTTON_PIN PIND
#define BUTTON_DDR DDRD

#define RIGHT_BUTTON_MASK (1<<DDD3)
#define LEFT_BUTTON_MASK  (1<<DDD4)
#define OK_BUTTON_MASK    (1<<DDD5)
#define BACK_BUTTON_MASK  (1<<DDD6)

#define BUTTON_MASK (RIGHT_BUTTON_MASK | LEFT_BUTTON_MASK | OK_BUTTON_MASK | BACK_BUTTON_MASK)

#define ENABLE_BUTTONS()	TIMER_releaseAsynchronousTimer()
#define DISABLE_BUTTONS()	TIMER_haltAsynchronousTimer()

// Variable to tell that the button is pressed (and debounced).
// Can be read with button_down() which will clear it.
extern volatile uint8_t buttons_down;

// Return non-zero if a button matching mask is pressed.
uint8_t button_down(uint8_t button_mask);

// Make button pins inputs and activate internal pullups.
void debounce_init(void);

// Decrease 2 bit vertical counter where mask = 1.
// Set counters to binary 11 where mask = 0.
#define VC_DEC_OR_SET(high, low, mask) \
				      low = ~(low & mask); \
				      high = low ^ (high & mask)
					  
// Check button state and set bits in the button_down variable if a
// debounced button down press is detected.
// Call this function every 10 ms or so.
static inline void debounce(void)
{
	// Eight vertical two bit counters for number of equal states
	static uint8_t vcount_low = 0xFF, vcount_high = 0xFF;
	
	// Keeps track of current (debounced) state
	static uint8_t button_state = 0;
	
	// Read buttons (active low so invert with ~). Xor with
	// button_state to see which ones are about to change state
	uint8_t state_changed = ~BUTTON_PIN ^ button_state;
	
	// Decrease counters where state_changed = 1, set the others to 0b11.
	VC_DEC_OR_SET(vcount_high, vcount_low, state_changed);
	
	// Update state_changed to have a 1 only if the counter overflowed
	state_changed &= vcount_low & vcount_high;
	
	// Change button_state for the buttons who’s counters rolled over
	button_state ^= state_changed;
	
	// Update button_down with buttons who’s counters rolled over
	// and who’s state is 1 (pressed)
	buttons_down |= button_state & state_changed;
}


#endif /* DEBOUNCE_H_ */