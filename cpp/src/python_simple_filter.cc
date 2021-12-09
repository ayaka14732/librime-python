#include <pybind11/embed.h>
#include <rime/candidate.h>
#include <rime/segmentation.h>
#include "python_simple_filter.h"

namespace pythonext {

// PythonSimpleFilter

PythonSimpleFilter::PythonSimpleFilter( const rime::Ticket& ticket, py::function py_entry )
    : Filter { ticket }, py_entry { py_entry } {}

rime::an<rime::Translation> PythonSimpleFilter::Apply( rime::an<rime::Translation> translation, rime::CandidateList* candidates ) {
    return rime::New<PythonSimpleFilterTranslation>( this, translation );
}

bool PythonSimpleFilter::FilterFunc( rime::an<rime::Candidate> cand ) noexcept {
    const std::string& text { cand->text() };

    try {
        return py_entry( text ).cast<bool>();
    } catch ( const py::error_already_set& e ) {
        LOG( ERROR ) << e.what();
    }

    return true;
}

// PythonSimpleFilterTranslation

PythonSimpleFilterTranslation::PythonSimpleFilterTranslation( PythonSimpleFilter* filter, rime::an<rime::Translation> translation )
    : filter_ { filter }, translation_ { translation } {
    LocateNextCandidate();
}

bool PythonSimpleFilterTranslation::Next() {
    if ( exhausted() )
        return false;
    if ( !translation_->Next() ) {
        set_exhausted( true );
        return false;
    }
    return LocateNextCandidate();
}

rime::an<rime::Candidate> PythonSimpleFilterTranslation::Peek() {
    return translation_->Peek();
}

bool PythonSimpleFilterTranslation::LocateNextCandidate() {
    while ( !translation_->exhausted() ) {
        const rime::an<rime::Candidate> cand { translation_->Peek() };
        if ( cand && filter_->FilterFunc( cand ) )
            return true;
        translation_->Next();
    }
    set_exhausted( true );
    return false;
}

}  // namespace pythonext
