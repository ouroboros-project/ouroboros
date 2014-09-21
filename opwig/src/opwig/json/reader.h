
#ifndef OPWIG_JSON_READER_H_
#define OPWIG_JSON_READER_H_

#include <opwig/md/ptr.h>
#include <opwig/md/namespace.h>

#include <string>
#include <istream>

namespace opwig {
namespace json {

class Reader {
public:
    Reader(std::istream& in, md::Ptr<md::Namespace> global);
    Reader(const std::string& s, md::Ptr<md::Namespace> global);

    /// Parses the json tree, populating the namespace.
    bool parse();

private:
    std::string contents_;
    md::Ptr<md::Namespace> global_;
};

} // namespace json
} // namespace opwig

#endif // OPWIG_JSON_READER_H_


