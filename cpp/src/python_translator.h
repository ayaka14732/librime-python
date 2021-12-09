#pragma once

#include <pybind11/embed.h>
#include <rime/common.h>
#include <rime/translation.h>
#include <rime/translator.h>

namespace py = pybind11;

namespace pythonext {

class PythonTranslator : public rime::Translator {
   public:
    PythonTranslator( const rime::Ticket& ticket, py::function py_entry );
    virtual rime::an<rime::Translation> Query( const std::string& input, const rime::Segment& segment ) noexcept override;

   private:
    const py::function py_entry;
    const py::object TranslatorQuery;
};

}  // namespace pythonext
