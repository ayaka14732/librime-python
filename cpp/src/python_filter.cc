#include <pybind11/embed.h>
#include <rime/candidate.h>
#include <rime/segmentation.h>
#include <rime/gear/translator_commons.h>
#include "python_filter.h"

namespace pythonext {

// PythonFilter

PythonFilter::PythonFilter( const rime::Ticket& ticket, py::function py_entry )
    : Filter { ticket }, py_entry { py_entry }, FilterQuery { py::eval( "rimeext.FilterQuery", py::globals(), py::globals() ) } {}

rime::an<rime::Translation> PythonFilter::Apply( rime::an<rime::Translation> translation, rime::CandidateList* candidates ) {
    return rime::New<PythonFilterTranslation>( this, translation );
}

void PythonFilter::FilterFunc( rime::an<rime::Candidate> cand, rime::CandidateQueue* cand_queue ) {
    const std::string& candidate_type { cand->type() };
    const std::string& text { cand->text() };
    const std::string comment { cand->comment() };  // return value is not a reference
    const std::string preedit { cand->preedit() };  // return value is not a reference

    try {
        const py::object filter_result { py_entry( FilterQuery( candidate_type, text, comment, preedit ) ) };

        // Case 1: Skip the current candidate. The candidate will be remained intact
        const bool should_skip { filter_result.attr( "should_skip" ).cast<bool>() };
        if ( should_skip ) {
            cand_queue->emplace_back( cand );
            return;
        }

        // Case 2: Remove the current candidate from the candidate list
        const bool should_remove { filter_result.attr( "should_remove" ).cast<bool>() };
        if ( should_remove )
            return;

        // Case 3: Change certain attributes of the current candidate
        const std::string new_candidate_type { filter_result.attr( "candidate_type" ).cast<std::string>() };
        const std::string new_text { filter_result.attr( "text" ).cast<std::string>() };
        const std::string new_comment { filter_result.attr( "comment" ).cast<std::string>() };

        const rime::an<rime::ShadowCandidate> new_cand { rime::New<rime::ShadowCandidate>( cand, new_candidate_type, new_text, new_comment ) };

        cand_queue->emplace_back( new_cand );
    } catch ( const py::error_already_set& e ) {
        LOG( ERROR ) << e.what();
    }
}

// PythonFilterTranslation

PythonFilterTranslation::PythonFilterTranslation( PythonFilter* filter, rime::an<rime::Translation> translation )
    : filter_ { filter }, PrefetchTranslation { translation } {}

bool PythonFilterTranslation::Replenish() {
    const rime::an<rime::Candidate> cand { translation_->Peek() };
    translation_->Next();
    filter_->FilterFunc( cand, &cache_ );
    return !cache_.empty();
}

}  // namespace pythonext
