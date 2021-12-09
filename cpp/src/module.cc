#include <pybind11/embed.h>
#include <filesystem>
#include <exception>
#include <rime/common.h>
#include <rime/registry.h>
#include <rime_api.h>
#include "python_simple_translator.h"
#include "python_translator.h"
#include "python_simple_filter.h"
#include "python_filter.h"

namespace py = pybind11;

template <typename T>
struct ComponentName { static const char* const name; };

template <>
const char* const ComponentName<pythonext::PythonSimpleTranslator>::name { "simple_translator" };
template <>
const char* const ComponentName<pythonext::PythonTranslator>::name { "translator" };
template <>
const char* const ComponentName<pythonext::PythonSimpleFilter>::name { "simple_filter" };
template <>
const char* const ComponentName<pythonext::PythonFilter>::name { "filter" };

/**
 * Determine the path to the Python script file according to the name string of a rime component.
 *
 * @param componentName The name of the component (e.g. translator, filter).
 * @param tagName The name string of the rime component.
 * @return Path to the Python script.
 * @exception Throws `std::runtime_error` if the file does not exist.
 */
static std::string getPythonScriptRealPath( const char* const componentName, const std::string& tagName ) {
    // determine the file name
    const std::string fileName { tagName + "." + componentName + ".py" };

    // the file should either exist in the user directory or the shared directory
    const std::filesystem::path userDir { std::filesystem::canonical( RimeGetUserDataDir() ) };
    const std::filesystem::path sharedDir { std::filesystem::canonical( RimeGetSharedDataDir() ) };
    const std::filesystem::path userFile { userDir / fileName };
    const std::filesystem::path sharedFile { sharedDir / fileName };
    const bool userFileExists { std::filesystem::exists( userFile ) };
    const bool sharedFileExists { std::filesystem::exists( sharedFile ) };

    if ( !userFileExists && !sharedFileExists ) {
        LOG( ERROR ) << "Expected file " << fileName << " in the user directory ("
                     << userDir.string() << ") or in the shared directory ("
                     << sharedDir.string() << ").";
        std::terminate();
    }

    // prefer the file in the user directory
    return ( userFileExists ? userFile : sharedFile ).string();
}

template <typename T>
class PythonComponent : public T::Component {
   public:
    PythonComponent() {};

    T* Create( const rime::Ticket& ticket ) {
        const rime::Ticket ticket_ { ticket.engine, ticket.name_space, ticket.name_space };
        const char* const componentName { ComponentName<T>::name };
        const std::string& tagName { ticket_.name_space };

        // load Python script file
        const std::string pythonScriptPath { getPythonScriptRealPath( componentName, tagName ) };
        LOG( INFO ) << "reading Python script file " << pythonScriptPath << ".";

        try {
            py::eval_file( pythonScriptPath, py::globals(), py::globals() );
            py::function py_entry { py::eval( "rime_main", py::globals(), py::globals() ) };
            return new T { ticket_, py_entry };
        } catch ( const py::error_already_set& e ) {
            LOG( ERROR ) << e.what();
            std::terminate();
        }
    }
};

static void rime_pythonext_initialize() {
    LOG( INFO ) << "registering components from module 'pythonext'.";

    py::initialize_interpreter();

    try {
        py::exec( "import rimeext", py::globals(), py::globals() );
    } catch ( const py::error_already_set& e ) {
        LOG( ERROR ) << e.what();
        std::terminate();
    }

    rime::Registry& r { rime::Registry::instance() };
    r.Register( "python_simple_translator", new PythonComponent<pythonext::PythonSimpleTranslator>() );
    r.Register( "python_translator", new PythonComponent<pythonext::PythonTranslator>() );
    r.Register( "python_simple_filter", new PythonComponent<pythonext::PythonSimpleFilter>() );
    r.Register( "python_filter", new PythonComponent<pythonext::PythonFilter>() );
}

static void rime_pythonext_finalize() {
    py::finalize_interpreter();
}

RIME_REGISTER_MODULE( pythonext )
