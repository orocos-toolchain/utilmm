#include "utilmm/configsearch/configuration_finder.hh"

#define WIN32_LEAN_AND_MEAN
#include <boost/program_options.hpp>
#if BOOST_VERSION >= 104600
#include <boost/filesystem.hpp>
#endif
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>

namespace utilmm {

const char* configEnv = "ROCK_CONFIG_PATH";


std::string ConfigurationFinder::find( const std::string& configFile)
{
	return find(configFile, "");
}

std::string ConfigurationFinder::find( const std::string& configFile, const std::string& packagename)
{
	std::string configurationDir = getenv(configEnv) ? getenv(configEnv) : "";

	std::vector<std::string> searchDirectories;
	if( configurationDir.empty() )
	{
		fprintf(stderr, "WARNING: ConfigurationFinder: environment variable %s is not set\n", configEnv);
	} else {

		boost::split(searchDirectories, configurationDir, boost::is_any_of(":"));
	}

	if(packagename != "")
	{
		// append packagename to configuration files, e.g. if packagename is my-package
		// my-package/ will be appended to the searchDirectories
		std::vector<std::string>::iterator it = searchDirectories.begin();
		for(;it != searchDirectories.end(); it++)
		{
			boost::filesystem::path directory(*it);
			boost::filesystem::path searchPath = operator/(directory, packagename);
			*it = searchPath.string();
		}
	}

	return search(configFile, searchDirectories);
}

std::string ConfigurationFinder::search(const std::string& file, const std::vector<std::string>& searchDirectories)
{
	std::string searchFile(file);
	std::string result;

	if( searchFile != "")
	{
		// Check current directory first
		if(boost::filesystem::exists(searchFile))
		{
			result =  boost::filesystem::system_complete( searchFile ).string();

		} else {
			// search in given search directories
			std::vector<std::string>::const_iterator it = searchDirectories.begin();

			for(; it != searchDirectories.end(); it++)
			{
				boost::filesystem::path dir(*it);
				boost::filesystem::path testPath = operator/(dir,searchFile);

				if( boost::filesystem::exists(testPath) )
				{
					result = boost::filesystem::system_complete( testPath ).string();
					break;
				}
			}
		}
	}

	return result;
}

std::string ConfigurationFinder::findSystemConfig(const std::string& file, const std::string& systemId)
{
	std::vector<std::string> result;
	boost::algorithm::split(result, systemId, boost::algorithm::is_any_of("_"));

	if(result.size() != 2)
	{
		fprintf(stderr, "WARNING: ConfigurationFinder: Invalid systemConfig provided\n"); 
		fprintf(stderr, "ConfigurationFinder: searching in basedir %s\n", systemId.c_str()); 
		
	}

	boost::filesystem::path baseDir(result[0]);
	boost::filesystem::path systemIdDir = operator/(baseDir, result[1]);
        // Split file name, when a <folder>/<file> is given  -- allows
        // search in given directories of the systemconfiguration directory
        // and keeps consistency with the ruby implementation 
        boost::algorithm::split(result, file, boost::algorithm::is_any_of("/"));
        boost::filesystem::path subdir;

        std::string searchFile = result.back();
        if(result.size() >= 2)
        {
            for(size_t i = 0; i < result.size() - 1; ++i)
            {
                subdir = operator/(subdir, result[i]);
            }

        }
    
        if(!subdir.string().empty())
        {
            systemIdDir = operator/(systemIdDir,subdir);
            baseDir = operator/(baseDir,subdir);
        }

	std::string systemIdConfig = find(searchFile,systemIdDir.string() );
	std::string baseConfig = find(searchFile, baseDir.string());

	if(systemIdConfig != "")
		return systemIdConfig;
	
	return baseConfig;
}

} // end namespace rock

