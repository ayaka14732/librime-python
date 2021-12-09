#pragma once

#include <pybind11/embed.h>
#include <rime/common.h>
#include <rime/filter.h>
#include <rime/gear/filter_commons.h>
#include <rime/translation.h>

namespace py = pybind11;

namespace pythonext {

class PythonSimpleFilter : public rime::Filter {
   public:
    PythonSimpleFilter( const rime::Ticket& ticket, py::function py_entry );
    virtual rime::an<rime::Translation> Apply( rime::an<rime::Translation> translation, rime::CandidateList* candidates ) override;
    bool FilterFunc( rime::an<rime::Candidate> cand ) noexcept;

   private:
    const py::function py_entry;
};

class PythonSimpleFilterTranslation : public rime::Translation {
   public:
    PythonSimpleFilterTranslation( PythonSimpleFilter* filter, rime::an<rime::Translation> translation );
    virtual bool Next() override;
    virtual rime::an<rime::Candidate> Peek() override;

   protected:
    bool LocateNextCandidate();
    rime::an<rime::Translation> translation_;
    PythonSimpleFilter* const filter_;
};

}  // namespace pythonext
