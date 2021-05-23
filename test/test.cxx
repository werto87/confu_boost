#include "confu_boost/confuBoost.hxx"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <catch2/catch.hpp>
#include <sstream>
#include <string>

BOOST_FUSION_DEFINE_STRUCT ((database), Character, (std::string, id) (std::string, positionId) (std::string, accountId))

BOOST_SERIALIZATION_BOILER_PLATE (database::Character)

namespace enumTest
{
enum struct SomeEnum
{
  value0,
  value1
};
}

BOOST_FUSION_DEFINE_STRUCT ((enumTest), StructWithEnum, (enumTest::SomeEnum, id))

BOOST_SERIALIZATION_BOILER_PLATE (enumTest::StructWithEnum)
// TODO test pair with optional

namespace durak
{
struct GameData
{
  std::vector<std::pair<int, boost::optional<int>>> table{};
};
}

BOOST_FUSION_ADAPT_STRUCT (durak::GameData, table)
BOOST_SERIALIZATION_BOILER_PLATE (durak::GameData)
namespace test
{

TEST_CASE ("struct to text")
{
  auto characterStringStream = std::stringstream{};
  boost::archive::text_oarchive characterArchive{ characterStringStream };
  characterArchive << database::Character{ .id = "id", .positionId = "positionId", .accountId = "accountId" };
  REQUIRE (characterStringStream.str () == "22 serialization::archive 18 0 0 2 id 10 positionId 9 accountId");
}

TEST_CASE ("struct to text with enum and toString function")
{
  auto object = durak::GameData{ .table = { std::pair<int, int>{ 13, 37 } } };
  auto objectAsString = confu_boost::toString (object);
  auto object1 = confu_boost::toObject<durak::GameData> (objectAsString);
  REQUIRE (object1.table.at (0).first == 13);
  REQUIRE (object1.table.at (0).second.value () == 37);
  REQUIRE (objectAsString == confu_boost::toString (object1));
}

TEST_CASE ("text to struct")
{
  auto characterStringStream = std::stringstream{};
  characterStringStream << "22 serialization::archive 18 0 0 2 id 10 positionId 9 accountId";
  boost::archive::text_iarchive ia (characterStringStream);
  auto character = database::Character{};
  ia >> character;
  REQUIRE (character.id == "id");
  REQUIRE (character.positionId == "positionId");
  REQUIRE (character.accountId == "accountId");
}

}
