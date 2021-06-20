#include "debounce.h"

// Bits is set to one if a depounced press is detected.
volatile uint8_t buttons_down;
// Return non-zero if a button matching mask is pressed.
uint8_t button_down(uint8_t button_mask)
{
	// ATOMIC_BLOCK is needed if debounce() is called from within an ISR
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		// And with debounced state for a one if they match
		button_mask &= buttons_down;
		// Clear if there was a match
		buttons_down ^= button_mask;
	}
	// Return non-zero if there was a match
	return button_mask;
}


BUTTON_ID get_pressed_button_id()
{
	if(button_down(RIGHT_BUTTON_MASK))
		return RIGHT_BUTTON_ID;
	
	else  if (button_down(LEFT_BUTTON_MASK))
		return LEFT_BUTTON_ID;
	
	else  if (button_down(OK_BUTTON_MASK))
		return OK_BUTTON_ID;
	
	else  if (button_down(BACK_BUTTON_MASK))
		return BACK_BUTTON_ID;
	
	return NO_BUTTON_PRESSED;
}

static inline void __resetButtons()
{
	button_down(LEFT_BUTTON_MASK);
	button_down(RIGHT_BUTTON_MASK);
	button_down(OK_BUTTON_MASK);
	button_down(BACK_BUTTON_MASK);
	button_down(SWITCH_BUTTON_MASK);
}

void debounce_init(void)
{
	// Button pins as input
	BUTTON_DDR &= ~(BUTTON_MASK);
	// Enable pullup on buttons
	BUTTON_PORT |= BUTTON_MASK;
	__resetButtons();
}

TIMER_CounterOverflow_Interrupt(2)
{
	debounce();
}