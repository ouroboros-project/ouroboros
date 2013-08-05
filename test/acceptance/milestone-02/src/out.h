
#ifndef OUROBOROS_MILESTONE_00_PROMPT_H_
#define OUROBOROS_MILESTONE_00_PROMPT_H_

namespace out {

/// Avtivates line breaking.
extern bool BREAK_LINE;

/// Prints prompt output.
void send_message (const char* str);

/// Prints n prompt output.
void send_multimessage (const char* str, int n);

namespace constants {

extern const char *TALKER_NAME;

} // namespace constants

} // namespace out

#endif

