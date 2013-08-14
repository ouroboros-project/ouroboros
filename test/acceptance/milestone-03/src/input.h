
#ifndef OUROBOROS_MILESTONE_PROMPT_IN_H_
#define OUROBOROS_MILESTONE_PROMPT_IN_H_

namespace input {

class Receiver {

  public:

    /// Receives a message from the input.
    const char* receive_message ();
    
    /// Receives a number from the input.
    double receive_number ();
    
    /// Receives a confirmation from the input.
    bool receive_confirmation ();

  private:

    char buffer[256];

};

} // namespace in

#endif

