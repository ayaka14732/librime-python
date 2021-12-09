#pragma once

#include <pybind11/embed.h>
#include <rime/common.h>
#include <rime/translation.h>
#include <rime/translator.h>

namespace py = pybind11;

namespace pythonext {

class PythonSimpleTranslator : public rime::Translator {
   public:
    PythonSimpleTranslator( const rime::Ticket& ticket, py::function py_entry );
    virtual rime::an<rime::Translation> Query( const std::string& input, const rime::Segment& segment ) noexcept override;

   private:
    const py::function py_entry;
};

}  // namespace pythonext
