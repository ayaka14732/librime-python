#include <pybind11/embed.h>
#include <rime/candidate.h>
#include <rime/segmentation.h>
#include "python_simple_translator.h"

namespace pythonext {

PythonSimpleTranslator::PythonSimpleTranslator( const rime::Ticket& ticket, py::function py_entry )
    : Translator { ticket }, py_entry { py_entry } {}

rime::an<rime::Translation> PythonSimpleTranslator::Query( const std::string& input, const rime::Segment& segment ) noexcept {
    if ( !segment.HasTag( "abc" ) )
        return nullptr;

    const size_t segment_start { segment.start };
    const size_t segment_end { segment.end };

    try {
        // skip if return value is None
        // Note: cannot assign to type `py::list` directly
        // https://pybind11.readthedocs.io/en/stable/advanced/pycpp/object.html#assigning-py-none-to-wrappers
        const py::object output { py_entry( input ) };
        if ( output.is_none() )
            return nullptr;

        const py::list output_list { output };

        // skip if returned list is empty
        if ( output_list.empty() )
            return nullptr;

        // create translation list from the returned list
        const rime::an<rime::FifoTranslation> translation { rime::New<rime::FifoTranslation>() };
        for ( const py::handle& item : output_list ) {
            const std::string item_string { item.cast<std::string>() };
            translation->Append( rime::New<rime::SimpleCandidate>( "python_simple", segment_start, segment_end, item_string ) );
        }
        return translation;
    } catch ( const py::error_already_set& e ) {
        LOG( ERROR ) << e.what();
    }

    return nullptr;
}

}  // namespace pythonext
