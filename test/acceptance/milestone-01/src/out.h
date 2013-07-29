
#ifndef OUROBOROS_MILESTONE_00_PROMPT_H_
#define OUROBOROS_MILESTONE_00_PROMPT_H_

namespace out {

/// Prints prompt output.
void send_message (const char* str);

/// Prints n prompt output.
void send_multimessage (const char* str, int n);

} // namespace out

#endif

