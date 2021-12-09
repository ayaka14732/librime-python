#include <pybind11/embed.h>
#include <rime/candidate.h>
#include <rime/segmentation.h>
#include "python_translator.h"

namespace pythonext {

PythonTranslator::PythonTranslator( const rime::Ticket& ticket, py::function py_entry )
    : Translator { ticket }, py_entry { py_entry }, TranslatorQuery { py::eval( "rimeext.TranslatorQuery", py::globals(), py::globals() ) } {}

rime::an<rime::Translation> PythonTranslator::Query( const std::string& input, const rime::Segment& segment ) noexcept {
    if ( !segment.HasTag( "abc" ) )
        return nullptr;

    const size_t segment_start { segment.start };

    try {
        // skip if return value is None
        const py::object output { py_entry( TranslatorQuery( input ) ) };
        if ( output.is_none() )
            return nullptr;

        const py::list output_list { output };

        // skip if returned list is empty
        if ( output_list.empty() )
            return nullptr;

        // create translation list from the returned list
        const rime::an<rime::FifoTranslation> translation { rime::New<rime::FifoTranslation>() };
        for ( const py::handle& candidate : output_list ) {
            // get all the 5 attributes from the object
            const std::string text { candidate.attr( "text" ).cast<std::string>() };
            const size_t length { candidate.attr( "length" ).cast<size_t>() };
            const std::string candidate_type { candidate.attr( "candidate_type" ).cast<std::string>() };
            const std::string comment { candidate.attr( "comment" ).cast<std::string>() };
            const std::string preedit { candidate.attr( "preedit" ).cast<std::string>() };

            translation->Append( rime::New<rime::SimpleCandidate>( candidate_type, segment_start, segment_start + length, text, comment, preedit ) );
        }
        return translation;
    } catch ( const py::error_already_set& e ) {
        LOG( ERROR ) << e.what();
    }

    return nullptr;
}

}  // namespace pythonext
