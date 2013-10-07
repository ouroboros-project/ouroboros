#ifndef OUROBOROS_UTILS_IDGENERATOR_H_
#define OUROBOROS_UTILS_IDGENERATOR_H_


#include <unordered_set>
#include <opa/utils/uncopyable.h>

namespace opa {
namespace utils {

/// Utility class to generate unique numeric IDs in a range of possible numbers.
/** This class is used to generate unique IDs, and contains a numeric (int) range, 
    and can return number by number of this range uniquely. It's possible to 
    'release' an ID, allowing it to be re-assigned. */
class IDGenerator : public Uncopyable {
  public:
    /// Initializes fields with given values.
    /** Initialize the ID Generator with range [min_id, max_id],
        and given error value.
    * @param min_id is the minimum ID value generated.
    * @param max_id is the maximum ID value generated.
    * @param error_value is the value that some methods return when an error occurred.
    */
    IDGenerator(int min_id, int max_id, int error_value);
    ~IDGenerator() {}

    /// Generate a new id.
    /** @return The new unique id. */
    int GenerateID();

    /// Release an id.
    /** @return An error value or the recieved id.*/
    int ReleaseID(int id);

    /// The error value for this generator.
    /** @return the error value for this generator.*/
    int error_value() const { return error_value_; }

  private:
    int                             current_id_;
    const int                       min_id_,
                                    max_id_,
                                    error_value_;
    std::unordered_set<int>    unused_ids_;
    size_t range () const { return static_cast<size_t>(max_id_ - min_id_ + 1);}
};

} // namespace util
} // namespace opa

#endif
