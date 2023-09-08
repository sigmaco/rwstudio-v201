#ifndef RWREG_H
#define RWREG_H

#include <string>
#include <vector>

namespace CSL {

void
RegSetString (const std::string& application,
              const std::string& section, 
              const std::string& entry,
              const std::string& value);

const std::string
RegGetString (const std::string& application,
              const std::string& section, 
              const std::string& entry,
              const std::string& defaultVal);

}

#endif