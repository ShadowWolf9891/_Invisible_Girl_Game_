/*  Copyright (C) 2018-2022 Davide Faconti, Eurecat -  All Rights Reserved
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
*   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
*   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <filesystem>
#include <fstream>
#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/utils/shared_library.h"
#include "behaviortree_cpp/xml_parsing.h"
#include "tinyxml2/tinyxml2.h"

#ifdef USING_ROS
#include <ros/package.h>
#endif

namespace BT
{
BehaviorTreeFactory::BehaviorTreeFactory()
{
  parser_ = std::make_shared<XMLParser>(*this);
  registerNodeType<FallbackNode>("Fallback");
  registerNodeType<SequenceNode>("Sequence");
  registerNodeType<SequenceWithMemory>("SequenceWithMemory");

#ifdef USE_BTCPP3_OLD_NAMES
  registerNodeType<SequenceWithMemory>("SequenceStar");   // backward compatibility
#endif

  registerNodeType<ParallelNode>("Parallel");
  registerNodeType<ReactiveSequence>("ReactiveSequence");
  registerNodeType<ReactiveFallback>("ReactiveFallback");
  registerNodeType<IfThenElseNode>("IfThenElse");
  registerNodeType<WhileDoElseNode>("WhileDoElse");

  registerNodeType<InverterNode>("Inverter");

  registerNodeType<RetryNode>("RetryUntilSuccessful");
  registerNodeType<KeepRunningUntilFailureNode>("KeepRunningUntilFailure");
  registerNodeType<RepeatNode>("Repeat");
  registerNodeType<TimeoutNode<>>("Timeout");
  registerNodeType<DelayNode>("Delay");

  registerNodeType<ForceSuccessNode>("ForceSuccess");
  registerNodeType<ForceFailureNode>("ForceFailure");

  registerNodeType<AlwaysSuccessNode>("AlwaysSuccess");
  registerNodeType<AlwaysFailureNode>("AlwaysFailure");
  registerNodeType<ScriptNode>("Script");

  registerNodeType<SubTreeNode>("SubTree");

  registerNodeType<PreconditionNode>("Precondition");

  registerNodeType<SwitchNode<2>>("Switch2");
  registerNodeType<SwitchNode<3>>("Switch3");
  registerNodeType<SwitchNode<4>>("Switch4");
  registerNodeType<SwitchNode<5>>("Switch5");
  registerNodeType<SwitchNode<6>>("Switch6");

#ifdef NCURSES_FOUND
  registerNodeType<ManualSelectorNode>("ManualSelector");
#endif
  for (const auto& it : builders_)
  {
    builtin_IDs_.insert(it.first);
  }

  scripting_enums_ = std::make_shared<std::unordered_map<std::string, int>>();
}

bool BehaviorTreeFactory::unregisterBuilder(const std::string& ID)
{
  if (builtinNodes().count(ID))
  {
    throw LogicError("You can not remove the builtin registration ID [", ID, "]");
  }
  auto it = builders_.find(ID);
  if (it == builders_.end())
  {
    return false;
  }
  builders_.erase(ID);
  manifests_.erase(ID);
  return true;
}

void BehaviorTreeFactory::registerBuilder(const TreeNodeManifest& manifest,
                                          const NodeBuilder& builder)
{
  auto it = builders_.find(manifest.registration_ID);
  if (it != builders_.end())
  {
    throw BehaviorTreeException("ID [", manifest.registration_ID, "] already registered");
  }

  builders_.insert({manifest.registration_ID, builder});
  manifests_.insert({manifest.registration_ID, manifest});
}

void BehaviorTreeFactory::registerSimpleCondition(
    const std::string& ID, const SimpleConditionNode::TickFunctor& tick_functor,
    PortsList ports)
{
  NodeBuilder builder = [tick_functor, ID](const std::string& name,
                                           const NodeConfig& config) {
    return std::make_unique<SimpleConditionNode>(name, tick_functor, config);
  };

  TreeNodeManifest manifest = {NodeType::CONDITION, ID, std::move(ports), {}};
  registerBuilder(manifest, builder);
}

void BehaviorTreeFactory::registerSimpleAction(
    const std::string& ID, const SimpleActionNode::TickFunctor& tick_functor,
    PortsList ports)
{
  NodeBuilder builder = [tick_functor, ID](const std::string& name,
                                           const NodeConfig& config) {
    return std::make_unique<SimpleActionNode>(name, tick_functor, config);
  };

  TreeNodeManifest manifest = {NodeType::ACTION, ID, std::move(ports), {}};
  registerBuilder(manifest, builder);
}

void BehaviorTreeFactory::registerSimpleDecorator(
    const std::string& ID, const SimpleDecoratorNode::TickFunctor& tick_functor,
    PortsList ports)
{
  NodeBuilder builder = [tick_functor, ID](const std::string& name,
                                           const NodeConfig& config) {
    return std::make_unique<SimpleDecoratorNode>(name, tick_functor, config);
  };

  TreeNodeManifest manifest = {NodeType::DECORATOR, ID, std::move(ports), {}};
  registerBuilder(manifest, builder);
}

void BehaviorTreeFactory::registerFromPlugin(const std::string& file_path)
{
  BT::SharedLibrary loader;
  loader.load(file_path);
  typedef void (*Func)(BehaviorTreeFactory&);

  if (loader.hasSymbol(PLUGIN_SYMBOL))
  {
    Func func = (Func)loader.getSymbol(PLUGIN_SYMBOL);
    func(*this);
  }
  else
  {
    std::cout << "ERROR loading library [" << file_path << "]: can't find symbol ["
              << PLUGIN_SYMBOL << "]" << std::endl;
  }
}

#ifdef USING_ROS

#ifdef _WIN32
const char os_pathsep(';');   // NOLINT
#else
const char os_pathsep(':');   // NOLINT
#endif

// This function is a copy from the one in class_loader_imp.hpp in ROS pluginlib
// package, licensed under BSD.
// https://github.com/ros/pluginlib
std::vector<std::string> getCatkinLibraryPaths()
{
  std::vector<std::string> lib_paths;
  const char* env = std::getenv("CMAKE_PREFIX_PATH");
  if (env)
  {
    const std::string env_catkin_prefix_paths(env);
    std::vector<BT::StringView> catkin_prefix_paths =
        splitString(env_catkin_prefix_paths, os_pathsep);
    for (BT::StringView catkin_prefix_path : catkin_prefix_paths)
    {
      std::filesystem::path path(static_cast<std::string>(catkin_prefix_path));
      std::filesystem::path lib("lib");
      lib_paths.push_back((path / lib).string());
    }
  }
  return lib_paths;
}

void BehaviorTreeFactory::registerFromROSPlugins()
{
  std::vector<std::string> plugins;
  ros::package::getPlugins("behaviortree_cpp", "bt_lib_plugin", plugins, true);
  std::vector<std::string> catkin_lib_paths = getCatkinLibraryPaths();

  for (const auto& plugin : plugins)
  {
    auto filename = std::filesystem::path(plugin + BT::SharedLibrary::suffix());
    for (const auto& lib_path : catkin_lib_paths)
    {
      const auto full_path = std::filesystem::path(lib_path) / filename;
      if (std::filesystem::exists(full_path))
      {
        std::cout << "Registering ROS plugins from " << full_path.string() << std::endl;
        registerFromPlugin(full_path.string());
        break;
      }
    }
  }
}
#else

void BehaviorTreeFactory::registerFromROSPlugins()
{
  throw RuntimeError("Using attribute [ros_pkg] in <include>, but this library was "
                     "compiled without ROS support. Recompile the BehaviorTree.CPP "
                     "using catkin");
}
#endif

void BehaviorTreeFactory::registerBehaviorTreeFromFile(const std::string& filename)
{
  parser_->loadFromFile(filename);
}

void BehaviorTreeFactory::registerBehaviorTreeFromText(const std::string& xml_text)
{
  parser_->loadFromText(xml_text);
}

std::vector<std::string> BehaviorTreeFactory::registeredBehaviorTrees() const
{
  return parser_->registeredBehaviorTrees();
}

void BehaviorTreeFactory::clearRegisteredBehaviorTrees()
{
  parser_->clearInternalState();
}

std::unique_ptr<TreeNode> BehaviorTreeFactory::instantiateTreeNode(
    const std::string& name, const std::string& ID, const NodeConfig& config) const
{
  auto it = builders_.find(ID);
  if (it == builders_.end())
  {
    std::cerr << ID << " not included in this list:" << std::endl;
    for (const auto& builder_it : builders_)
    {
      std::cerr << builder_it.first << std::endl;
    }
    throw RuntimeError("BehaviorTreeFactory: ID [", ID, "] not registered");
  }

  std::unique_ptr<TreeNode> node = it->second(name, config);
  node->setRegistrationID(ID);
  node->config_.enums = scripting_enums_;

  auto AssignConditions = [](auto& conditions, auto& excutors) {
    for (const auto& [cond_id, script] : conditions)
    {
      if (auto executor = ParseScript(script))
      {
        excutors[size_t(cond_id)] = executor.value();
      }
      else
      {
        throw LogicError("Error in the script \"", script, "\"\n", executor.error());
      }
    }
  };
  AssignConditions(config.pre_conditions, node->pre_parsed_);
  AssignConditions(config.post_conditions, node->post_parsed_);

  return node;
}

const std::unordered_map<std::string, NodeBuilder>& BehaviorTreeFactory::builders() const
{
  return builders_;
}

const std::unordered_map<std::string, TreeNodeManifest>&
BehaviorTreeFactory::manifests() const
{
  return manifests_;
}

const std::set<std::string>& BehaviorTreeFactory::builtinNodes() const
{
  return builtin_IDs_;
}

Tree BehaviorTreeFactory::createTreeFromText(const std::string& text,
                                             Blackboard::Ptr blackboard)
{
  XMLParser parser(*this);
  parser.loadFromText(text);
  auto tree = parser.instantiateTree(blackboard);
  tree.manifests = this->manifests();
  return tree;
}

Tree BehaviorTreeFactory::createTreeFromFile(const std::string& file_path,
                                             Blackboard::Ptr blackboard)
{
  XMLParser parser(*this);
  parser.loadFromFile(file_path);
  auto tree = parser.instantiateTree(blackboard);
  tree.manifests = this->manifests();
  return tree;
}

Tree BehaviorTreeFactory::createTree(const std::string& tree_name,
                                     Blackboard::Ptr blackboard)
{
  auto tree = parser_->instantiateTree(blackboard, tree_name);
  tree.manifests = this->manifests();
  return tree;
}

void BehaviorTreeFactory::addDescriptionToManifest(const std::string& node_id,
                                                   const std::string& description)
{
  auto it = manifests_.find(node_id);
  if (it == manifests_.end())
  {
    throw std::runtime_error("addDescriptionToManifest: wrong ID");
  }
  it->second.description = description;
}

void BehaviorTreeFactory::registerScriptingEnum(StringView name, int value)
{
  (*scripting_enums_)[std::string(name)] = value;
}

TreeNode* Tree::rootNode() const
{
  if (subtrees.empty())
  {
    return nullptr;
  }
  auto& subtree_nodes = subtrees.front()->nodes;
  return subtree_nodes.empty() ? nullptr : subtree_nodes.front().get();
}

void Tree::sleep(std::chrono::system_clock::duration timeout)
{
  wake_up_->waitFor(timeout);
}

Tree::~Tree()
{
  haltTree();
}

NodeStatus Tree::tickOnce()
{
  return tickRoot(ONCE, {});
}

NodeStatus Tree::tickWhileRunning(std::chrono::milliseconds sleep_time)
{
  return tickRoot(WHILE_RUNNING, sleep_time);
}

Blackboard::Ptr Tree::rootBlackboard()
{
  if (subtrees.size() > 0)
  {
    return subtrees.front()->blackboard;
  }
  return {};
}

void Tree::applyVisitor(const std::function<void(const TreeNode*)>& visitor)
{
  for (auto const& subtree : subtrees)
  {
    BT::applyRecursiveVisitor(static_cast<const TreeNode*>(subtree->nodes.front().get()),
                              visitor);
  }
}

void Tree::applyVisitor(const std::function<void(TreeNode*)>& visitor)
{
  for (auto const& subtree : subtrees)
  {
    BT::applyRecursiveVisitor(static_cast<TreeNode*>(subtree->nodes.front().get()),
                              visitor);
  }
}

NodeStatus Tree::tickRoot(TickOption opt, std::chrono::milliseconds sleep_time)
{
  NodeStatus status = NodeStatus::IDLE;

  while (status == NodeStatus::IDLE ||
         (opt == TickOption::WHILE_RUNNING && status == NodeStatus::RUNNING))
  {
    if (!wake_up_)
    {
      initialize();
    }

    if (!rootNode())
    {
      throw RuntimeError("Empty Tree");
    }
    status = rootNode()->executeTick();
    if (status == NodeStatus::SUCCESS || status == NodeStatus::FAILURE)
    {
      rootNode()->resetStatus();
    }
    if (status == NodeStatus::RUNNING)
    {
      sleep(std::chrono::milliseconds(sleep_time));
    }
  }

  return status;
}

}   // namespace BT
