
void		addSearchPath(const char* path);
void		clearSearchPaths();
FILE*		findFile(const char* path, bool binary = true);
const char*	findPath(const char* path);

/**
Search for the speficied path in the current directory. If not found, move up in the folder hierarchy
until the path can be found or until the specified maximum number of steps is reached.

\note On consoles no recursive search will be done

\param	[in] path The path to look for
\param	[out] buffer Buffer to store the (potentially) adjusted path
\param	[in] bufferSize Size of buffer
\param	[in] maxRecursion Maximum number steps to move up in the folder hierarchy
return	true if path was found
*/
bool		searchForPath(const char* path, char* buffer, int bufferSize, bool isReadOnly, int maxRecursion);