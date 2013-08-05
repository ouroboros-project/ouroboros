
#ifndef OUROBOROS_MILESTONE_PROMPT_IN_H_
#define OUROBOROS_MILESTONE_PROMPT_IN_H_

namespace input {

/// Receives a message from the input.
const char* receive_message ();

/// Receives a number from the input.
double receive_number ();

/// Receives a confirmation from the input.
bool receive_confirmation ();

} // namespace in

#endif

