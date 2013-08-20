
#ifndef OUROBOROS_MILESTONE_00_PROMPT_H_
#define OUROBOROS_MILESTONE_00_PROMPT_H_

namespace out {


class Sender {

  public:

    /// Avtivates line breaking.
    bool break_line;

    /// Constructor.
    Sender();
    
    /// Prints prompt output.
    void send_message (const char* str);
    
    /// Prints n prompt output.
    void send_multimessage (const char* str, int n);

  private:

    const char *const TALKER_NAME;

};

} // namespace out

#endif

