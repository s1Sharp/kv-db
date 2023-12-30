#include "db_parser.h"


#include "y.tab.hpp"

extern int run_parser();

namespace project {
	void serveParcer() {
		run_parser();
	}
}  // namespace project
