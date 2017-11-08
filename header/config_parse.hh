#ifndef __CONFIG_PARSE_HH__
#define __CONFIG_PARSE_HH__

#include<string>
#include<map>
#include<exception>
#include<sstream>

class element_not_found : public std::exception{
private:
  std::string mMsg;

public:
  element_not_found( const std::string& name, const std::string& value );

  const char* what() const noexcept override;
};

/*! Configuration file parser
 *
 * Data to be provided via an ini file can be described using add_element
 * function calls.  The file can then be parsed using parse_config, and
 * finally data retrieved via get_element.
 *
 * Configuration is described by add_element.  The first argument to add_element
 * describes a word to be found between square brackets, [].  The second
 * argument describes pieces found under that tag, using an = sign.  i.e.:
 * 
 * @code{.ini}
 * [FOO]
 *   bar = test 
 *   value = 42 
 * @endcode
 *
 * The above would be described as:
 * @code{.cpp}
 * config_parse cp;
 *
 * cp.add_element( "FOO", "bar" );
 * cp.add_element( "FOO", "value" );
 *
 * cp.parse_config( "filename.ini" );
 *
 * assert( cp.get_element<string>( "FOO", "bar" ) == "test" );
 * assert( cp.get_element<int>( "FOO", "value" ) == 42 );
 * @endcode
 */
class config_parse{
private:
  typedef std::map<std::string, std::string> element;

  std::map<std::string, element> mElements;

public:
  /*! Provide names of config data to look for
   *
   * @param arg  Tag name to look for between square brackets
   *
   * @param eleName  Element to look for under specified tag
   *
   */
  void add_element( const std::string& arg, const std::string& eleName );

  /*! Retrieve element from parsed data
   *
   * @tparam T  Type data should be translated into via stream extraction
   *
   * @param tag  Name of tag (value in square brackets) to get data from
   *
   * @param name  Name of element under specified tag
   *
   * Throws an error if configuration at tag and name were not declared.  Data
   * t of type T is created via default constructor, and then data is put into t
   * using stream extraction.
   */
  template<typename T>
  T get_element( const std::string& tag, const std::string& name ){
    std::string text;

    try{
      text = mElements.at( tag ).at( name );
    } catch( std::out_of_range ){
      throw element_not_found( tag, name );
    }

    std::stringstream ss( text );
    T t;

    ss >> t;

    return t;
  }

  /*! Parse file for config previously specified elements
   *
   * @param fileName  Name of file to be parsed
   *
   * Searches for data specified inside fileName file that was described using
   * add_element function
   */
  void parse_config( const std::string& fileName );
};

#endif

