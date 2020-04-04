#include "Service.h"


#include <fstream>


namespace M3d {

	// Attention a ne pas l'utiliser dans un autre objet static !!!

	std::ostream& Service::Warning = *(new std::ofstream("warning_null.tmp")); // not open 
	
}
