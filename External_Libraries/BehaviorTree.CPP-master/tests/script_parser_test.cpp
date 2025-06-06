#include <gtest/gtest.h>

#include "behaviortree_cpp/scripting/operators.hpp"
#include "behaviortree_cpp/bt_factory.h"
#include <lexy/input/string_input.hpp>

TEST(ParserTest, AnyTypes)
{
  auto Parse = [](const char* str) {
    return lexy::parse<BT::Grammar::AnyValue>(lexy::zstring_input(str),
                                              lexy_ext::report_error);
  };

  auto result = Parse("628");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<int>(), 628);

  result = Parse("-628");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<int>(), -628);

  result = Parse("0x100");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<int>(), 256);

  result = Parse("0X100");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<int>(), 256);

  result = Parse("3.14");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<double>(), 3.14);

  result = Parse("-3.14");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<double>(), -3.14);

  result = Parse("3.14e2");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<double>(), 314);

  result = Parse("3.14e-2");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<double>(), 0.0314);

  result = Parse("3e2");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<double>(), 300);

  result = Parse("3e-2");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<double>(), 0.03);

  result = Parse("'hello world '");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<std::string>(), "hello world ");

  result = Parse("true");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<int>(), 1);

  result = Parse("false");
  EXPECT_TRUE(result.is_success());
  EXPECT_EQ(result.value().cast<int>(), 0);
}

TEST(ParserTest, AnyTypes_Failing)
{
  std::istringstream null_sink;

  auto Parse = [](const char* str) {
    return lexy::parse<BT::Grammar::AnyValue>(lexy::zstring_input(str),
                                              lexy_ext::report_error);
  };

  EXPECT_TRUE(!Parse("0X100g").is_success());

  EXPECT_TRUE(!Parse("0X100.").is_success());

  EXPECT_TRUE(!Parse("3foo").is_success());

  EXPECT_TRUE(!Parse("65.").is_success());

  EXPECT_TRUE(!Parse("65.43foo").is_success());

  EXPECT_TRUE(!Parse("foo").is_success());
}

TEST(ParserTest, Equations)
{
  BT::Ast::Environment environment = {BT::Blackboard::create(), {}};

  auto GetResult = [&](const char* text) -> BT::Any {
    auto input = lexy::zstring_input<lexy::utf8_encoding>(text);
    auto result = lexy::parse<BT::Grammar::stmt>(input, lexy_ext::report_error);

    if (result.has_value())
    {
      auto exprs = LEXY_MOV(result).value();
      for (auto i = 0u; i < exprs.size() - 1; ++i)
      {
        exprs[i]->evaluate(environment);
      }
      return exprs.back()->evaluate(environment);
    }
    else
    {
      return {};
    }
  };
  //-------------------
  const auto& variables = environment.vars;
  EXPECT_EQ(GetResult("x:= 3; y:=5; x+y").cast<double>(), 8.0);
  EXPECT_EQ(variables->getKeys().size(), 2);
  EXPECT_EQ(variables->get<double>("x"), 3.0);
  EXPECT_EQ(variables->get<double>("y"), 5.0);

  EXPECT_EQ(GetResult("x+=1").cast<double>(), 4.0);
  EXPECT_EQ(variables->get<double>("x"), 4.0);

  EXPECT_EQ(GetResult("x-=1").cast<double>(), 3.0);
  EXPECT_EQ(variables->get<double>("x"), 3.0);

  EXPECT_EQ(GetResult("x*=2").cast<double>(), 6.0);
  EXPECT_EQ(variables->get<double>("x"), 6.0);

  EXPECT_EQ(GetResult("-x").cast<double>(), -6.0);

  EXPECT_EQ(GetResult("x/=2").cast<double>(), 3.0);
  EXPECT_EQ(variables->get<double>("x"), 3.0);

  EXPECT_EQ(GetResult("y").cast<double>(), 5.0);
  EXPECT_EQ(GetResult("y/2").cast<double>(), 2.5);
  EXPECT_EQ(GetResult("y*2").cast<double>(), 10.0);
  EXPECT_EQ(GetResult("y-x").cast<double>(), 2.0);

  EXPECT_EQ(GetResult("y & x").cast<double>(), (5 & 3));
  EXPECT_EQ(GetResult("y | x").cast<double>(), (5 | 3));
  EXPECT_EQ(GetResult("y ^ x").cast<double>(), (5 ^ 3));

  EXPECT_ANY_THROW(GetResult("y ^ 5.1").cast<double>());

  // test string variables
  EXPECT_EQ(GetResult("A:='hello'; B:=' '; C:='world'; A+B+C").cast<std::string>(), "hell"
                                                                                    "o "
                                                                                    "worl"
                                                                                    "d");
  EXPECT_EQ(variables->getKeys().size(), 5);
  EXPECT_EQ(variables->get<std::string>("A"), "hello");
  EXPECT_EQ(variables->get<std::string>("B"), " ");
  EXPECT_EQ(variables->get<std::string>("C"), "world");

  // check that whitespaces are handled correctly
  EXPECT_TRUE(!GetResult("A= '   right'; "
                         "B= ' center '; "
                         "C= 'left    '  ")
                   .empty());

  EXPECT_EQ(variables->getKeys().size(), 5);
  EXPECT_EQ(variables->get<std::string>("A"), "   right");
  EXPECT_EQ(variables->get<std::string>("B"), " center ");
  EXPECT_EQ(variables->get<std::string>("C"), "left    ");

  // can't change the type, once created
  EXPECT_ANY_THROW(GetResult("x=A"));
  EXPECT_ANY_THROW(GetResult("x='msg'"));
  EXPECT_ANY_THROW(GetResult("A=1.0"));

  // Invalid assignments
  EXPECT_ANY_THROW(GetResult(" 'hello' = 'world' "));
  EXPECT_ANY_THROW(GetResult(" 'hello' = 2.0 "));
  EXPECT_ANY_THROW(GetResult(" 3.0 = 2.0 "));

  size_t prev_size = variables->getKeys().size();
  EXPECT_ANY_THROW(GetResult("new_var=69"));
  EXPECT_EQ(variables->getKeys().size(), prev_size);   // shouldn't increase

  // check comparisons
  EXPECT_EQ(GetResult("x < y").cast<int>(), 1);
  EXPECT_EQ(GetResult("y > x").cast<int>(), 1);
  EXPECT_EQ(GetResult("y != x").cast<int>(), 1);
  EXPECT_EQ(GetResult("y == x").cast<int>(), 0);

  EXPECT_EQ(GetResult(" 'hello' == 'hello'").cast<int>(), 1);
  EXPECT_EQ(GetResult(" 'hello' != 'world'").cast<int>(), 1);
  EXPECT_EQ(GetResult(" 'hello' < 'world'").cast<int>(), 1);
  EXPECT_EQ(GetResult(" 'hello' > 'world'").cast<int>(), 0);

  EXPECT_NE(GetResult("x > y").cast<int>(), 1);
  EXPECT_NE(GetResult("y < x").cast<int>(), 1);
  EXPECT_NE(GetResult("y == x").cast<int>(), 1);

  EXPECT_EQ(GetResult("y == x ? 'T' : 'F'").cast<std::string>(), "F");
  EXPECT_EQ(GetResult("y != x ? 'T' : 'F'").cast<std::string>(), "T");

  EXPECT_EQ(GetResult("y == x").cast<int>(), 0);
  EXPECT_EQ(GetResult("y == 5").cast<int>(), 1);
  EXPECT_EQ(GetResult("x == 3").cast<int>(), 1);

  EXPECT_EQ(GetResult(" true ").cast<int>(), 1);
  EXPECT_EQ(GetResult(" 'true' ").cast<std::string>(), "true");

  GetResult("v1:= true; v2:= false");
  EXPECT_EQ(variables->get<int>("v1"), 1);
  EXPECT_EQ(variables->get<int>("v2"), 0);

  EXPECT_EQ(GetResult(" v2 = !false ").cast<int>(), 1);
  EXPECT_EQ(GetResult(" v2 = !v2 ").cast<int>(), 0);

  EXPECT_EQ(GetResult("v1 && v2").cast<int>(), 0);
  EXPECT_EQ(GetResult("v1 || v2").cast<int>(), 1);

  EXPECT_EQ(GetResult("(y == x) && (x == 3)").cast<int>(), 0);
  EXPECT_EQ(GetResult("(y == x) || (x == 3)").cast<int>(), 1);

  EXPECT_EQ(GetResult(" y == x  &&  x == 3 ").cast<int>(), 0);
  EXPECT_EQ(GetResult(" y == x  ||  x == 3 ").cast<int>(), 1);
}

TEST(ParserTest, Enums)
{
  BT::BehaviorTreeFactory factory;

  const std::string xml_text = R"(

    <root main_tree_to_execute = "MainTree" >
        <BehaviorTree ID="MainTree">
            <Script code = "A:=THE_ANSWER; color1:=RED; color2:=BLUE; color3:=GREEN" />
        </BehaviorTree>
    </root>)";

  enum Color
  {
    RED = 1,
    BLUE = 3,
    GREEN = 5
  };

  factory.registerScriptingEnum("THE_ANSWER", 42);
  factory.registerScriptingEnums<Color>();

  auto tree = factory.createTreeFromText(xml_text);
  const auto status = tree.tickWhileRunning();
  ASSERT_EQ(status, BT::NodeStatus::SUCCESS);

  const auto& blackboard = tree.subtrees.front()->blackboard;
  ASSERT_EQ(blackboard->get<int>("A"), 42);
  ASSERT_EQ(blackboard->get<int>("color1"), RED);
  ASSERT_EQ(blackboard->get<int>("color2"), BLUE);
  ASSERT_EQ(blackboard->get<int>("color3"), GREEN);
}
