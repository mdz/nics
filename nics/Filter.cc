#include <sstream>

#include "Filter.h"
#include "NICSCore.h"
#include "NICSGuile.h"
#include "NICSLog.h"

bool Filter::eval( const BoundVars& bindings ) const throw(Error) {

  // Check that a binding has been supplied for all possible variables

  for( LegalVars::const_iterator i = legalvars.begin() ;
       i != legalvars.end() ;
       ++i ) {

    BoundVars::const_iterator binding = bindings.find(i->first);

    if ( binding == bindings.end() || binding->second.type != i->second ) {
      NICSCore::log->log( NICSLog::Internal, NICSLog::Error,
			  "Missing or invalid binding supplied for legal variable "
			  + i->first );
      return false;
    }
  }

  ostringstream expr;
  expr << "(let (" << endl;
  for( BoundVars::const_iterator i = bindings.begin() ;
       i != bindings.end() ;
       ++i ) {
    expr << "    (" << i->first << " ";

    switch (i->second.type) {
    case variable::Integer:
      expr << i->second.intval;break;
    case variable::String:
      expr << '"' << i->second.stringval << '"';break;
    case variable::Boolean:
      expr << i->second.boolval ? "#t" : "#f";break;
    default:
      NICSCore::log->log( NICSLog::Internal, NICSLog::Error,
			  "Unknown type for variable " + i->first );
      return false;
    }

    expr << ")" << endl;
  }

  expr << "  )\n  " << scheme_text << ")";

  cerr << expr.str() << endl;

  return NICSCore::guile->eval_bool(expr.str());
}
