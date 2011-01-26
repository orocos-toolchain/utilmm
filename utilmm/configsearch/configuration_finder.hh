#ifndef _UTILMM_CONFIGURATION_FINDER_H_
#define _UTILMM_CONFIGURATION_FINDER_H_

#include <string>
#include <vector>

namespace utilmm {

/**
* Search for configuration files available within the current directory and 
* pathes given by the ROCK_CONFIG_PATH variable
*
*/
class ConfigurationFinder
{

public: 
	/**
	* Search for a file by name (no recursive search)
	* \return The full path once the file was found, otherwise an empty string
	*
	*/
	static std::string find(const std::string& file);

	/**
	* Search for a file within [ $ROCK_CONFIG_PATH ]/<packagename>/ (no recursive search)
	* \return The full path once the file was found, otherwise an empty string
	*/
	static std::string find(const std::string& file, const std::string& packagename);

	/**
	* Search for a file within some given directories
	* \return The full path once the file was found, otherwise an empty string
	*/
	static std::string search(const std::string& file, const std::vector<std::string>& searchDirectories);

};

}

#endif // _UTILMM_CONFIGURATION_FINDER_H_
