#ifndef AEB527E1_1664_4D1C_9D70_BFD5E18095D6
#define AEB527E1_1664_4D1C_9D70_BFD5E18095D6

#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/fusion/algorithm/query/count.hpp>
#include <boost/fusion/functional.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/count.hpp>
#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/intrinsic_fwd.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/type_index.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <exception>
#include <filesystem>
#include <iostream>
#include <istream>
#include <magic_enum.hpp>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
template <class Archive, typename T>
void
boostSerializationHelper (Archive &ar, T &t)
{
  boost::fusion::for_each (boost::mpl::range_c<unsigned, 0, boost::fusion::result_of::size<T>::value> (), [&] (auto index) {
    ar &boost::fusion::at_c<index> (t);
  });
}

// dont use BOOST_SERIALIZATION_BOILER_PLATE in a namespace
// clang-format off
#ifndef BOOST_SERIALIZATION_BOILER_PLATE
#define BOOST_SERIALIZATION_BOILER_PLATE(NAMESPACE_WITH_TYPE)         \
  namespace boost::serialization                                      \
  {                                                                   \
  template <class Archive>                                            \
  void                                                                \
  serialize (Archive &ar, NAMESPACE_WITH_TYPE &t, const unsigned int) \
  {                                                                   \
    boostSerializationHelper (ar, t);                                 \
  }                                                                   \
  }
#endif
// clang-format on
namespace confu_boost
{

template <typename TypeToTransform>
std::string
toString (TypeToTransform const &typeToTransform)
{
  auto typeToTransformStream = std::stringstream{};
  boost::archive::text_oarchive typeToTransformArchive{ typeToTransformStream };
  typeToTransformArchive << typeToTransform;
  return typeToTransformStream.str ();
}

template <typename TypeToTransform>
TypeToTransform
toObject (std::string const &objectAsString)
{
  auto stringStream = std::stringstream{};
  stringStream << objectAsString;
  boost::archive::text_iarchive ia (stringStream);
  auto typeToTransform = TypeToTransform{};
  ia >> typeToTransform;
  return typeToTransform;
}
}
#endif /* AEB527E1_1664_4D1C_9D70_BFD5E18095D6 */
