#ifndef OUROBOROS_UTILS_IDGENERATOR_H_
#define OUROBOROS_UTILS_IDGENERATOR_H_


#include <unordered_set>
#include <opa/utils/uncopyable.h>

namespace opa {
namespace utils {
class IDGenerator : public Uncopyable {
  public:
    IDGenerator(int min_id, int max_id, int error_value);
    ~IDGenerator() {}

    /// Generate a new id.
    /** @return The new unique id. */
    int GenerateID();

    /// Release an id.
    /** @return An error value or the recieved id.*/
    int ReleaseID(int id);

    /// The error value for this generator.
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