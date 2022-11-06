/** @file "/owlcpp/include/owlcpp/terms/ns_iri_tags.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012-3
*******************************************************************************/
#ifndef NS_IRI_TAGS_HPP_
#define NS_IRI_TAGS_HPP_

/**@brief Define namespace IRIs standard in OWL.
@details This macro is used for generating types and typelists.
((class name)("prefix")("IRI"))
*******************************************************************************/
#define OWLCPP_NAMESPACE_IRI_LIST \
   ((empty)("")("")) \
   ((blank)("_")("_")) \
   ((rdf)("rdf")("http://www.w3.org/1999/02/22-rdf-syntax-ns")) \
   ((rdfs)("rdfs")("http://www.w3.org/2000/01/rdf-schema")) \
   ((xsd)("xsd")("http://www.w3.org/2001/XMLSchema")) \
   ((owl)("owl")("http://www.w3.org/2002/07/owl")) \
/* */

#include "owlcpp/terms/ns_iri_tag_gen_macro.hpp"
#include "boost/preprocessor/seq/size.hpp"
#define OWLCPP_NAMESPACE_TAGS_TOTAL BOOST_PP_SEQ_SIZE(OWLCPP_NAMESPACE_IRI_LIST)

namespace owlcpp{ namespace terms{

/**@struct xxx
@brief Namespace IRI tag classes generated by macro OWLCPP_GENERATE_NAMESPACE_TAGS
@details Declare types for each standard namespace IRI, e.g.: @code
 struct rdfs {
   static const unsigned index = 3;
   static std::string const & iri() {
      static const std::string str="http://www.w3.org/2000/01/rdf-schema";
      return str;
   }
   static std::string const & prefix() {
      static const std::string str="rdfs";
      return str;
   }
   typedef ::owlcpp::Ns_id id_type;
   static id_type id() {return id_type(index);}
}; @endcode
*******************************************************************************/
OWLCPP_GENERATE_NAMESPACE_TAGS(OWLCPP_NAMESPACE_IRI_LIST, 0)

}//namespace terms


/**
*******************************************************************************/
inline bool is_empty(const Ns_id id) {return id == terms::empty::id();}

/**
*******************************************************************************/
inline bool is_blank(const Ns_id id) {return id == terms::blank::id();}

/**
*******************************************************************************/
inline bool is_iri(const Ns_id id) {
   return id != terms::empty::id() && id != terms::blank::id();
}

}//namespace owlcpp
#endif /* NS_IRI_TAGS_HPP_ */
