#pragma once

#include <pybind11/embed.h>
#include <rime/common.h>
#include <rime/filter.h>
#include <rime/gear/filter_commons.h>
#include <rime/translation.h>

namespace py = pybind11;

namespace pythonext {

class PythonFilter : public rime::Filter {
   public:
    PythonFilter( const rime::Ticket& ticket, py::function py_entry );
    virtual rime::an<rime::Translation> Apply( rime::an<rime::Translation> translation, rime::CandidateList* candidates ) override;
    void FilterFunc( rime::an<rime::Candidate> cand, rime::CandidateQueue* cand_queue );

   private:
    const py::function py_entry;
    const py::object FilterQuery;
};

class PythonFilterTranslation : public rime::PrefetchTranslation {
   public:
    PythonFilterTranslation( PythonFilter* filter, rime::an<rime::Translation> translation );

   protected:
    virtual bool Replenish() override;
    PythonFilter* const filter_;
};

}  // namespace pythonext
