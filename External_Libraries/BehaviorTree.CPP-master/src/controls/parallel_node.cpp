/* Copyright (C) 2015-2018 Michele Colledanchise -  All Rights Reserved
 * Copyright (C) 2018-2020 Davide Faconti, Eurecat -  All Rights Reserved
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

#include <algorithm>
#include <cstddef>

#include "behaviortree_cpp/controls/parallel_node.h"

namespace BT
{
constexpr const char* ParallelNode::THRESHOLD_FAILURE;
constexpr const char* ParallelNode::THRESHOLD_SUCCESS;

ParallelNode::ParallelNode(const std::string& name) :
  ControlNode::ControlNode(name, {}),
  success_threshold_(-1),
  failure_threshold_(1),
  read_parameter_from_ports_(false)
{
  setRegistrationID("Parallel");
}

ParallelNode::ParallelNode(const std::string& name, const NodeConfig& config) :
  ControlNode::ControlNode(name, config),
  success_threshold_(-1),
  failure_threshold_(1),
  read_parameter_from_ports_(true)
{}

NodeStatus ParallelNode::tick()
{
  if (read_parameter_from_ports_)
  {
    if (!getInput(THRESHOLD_SUCCESS, success_threshold_))
    {
      throw RuntimeError("Missing parameter [", THRESHOLD_SUCCESS, "] in ParallelNode");
    }

    if (!getInput(THRESHOLD_FAILURE, failure_threshold_))
    {
      throw RuntimeError("Missing parameter [", THRESHOLD_FAILURE, "] in ParallelNode");
    }
  }

  size_t success_childred_num = 0;
  size_t failure_childred_num = 0;

  const size_t children_count = children_nodes_.size();

  if (children_count < successThreshold())
  {
    throw LogicError("Number of children is less than threshold. Can never succeed.");
  }

  if (children_count < failureThreshold())
  {
    throw LogicError("Number of children is less than threshold. Can never fail.");
  }

  // Routing the tree according to the sequence node's logic:
  for (size_t i = 0; i < children_count; i++)
  {
    TreeNode* child_node = children_nodes_[i];

    bool const in_skip_list = (skip_list_.count(i) != 0);

    NodeStatus const prev_status = child_node->status();
    NodeStatus const child_status =
        in_skip_list ? prev_status : child_node->executeTick();

    // switch to RUNNING state as soon as you find an active child
    if (child_status != NodeStatus::SKIPPED)
    {
      setStatus(NodeStatus::RUNNING);
    }

    switch (child_status)
    {
      case NodeStatus::SUCCESS: {
        skip_list_.insert(i);
        success_childred_num++;

        if (success_childred_num == successThreshold())
        {
          skip_list_.clear();
          haltChildren();
          return NodeStatus::SUCCESS;
        }
      }
      break;

      case NodeStatus::FAILURE: {
        skip_list_.insert(i);
        failure_childred_num++;

        // It fails if it is not possible to succeed anymore or if
        // number of failures are equal to failure_threshold_
        if ((failure_childred_num > children_count - successThreshold()) ||
            (failure_childred_num == failureThreshold()))
        {
          skip_list_.clear();
          haltChildren();
          return NodeStatus::FAILURE;
        }
      }
      break;

      case NodeStatus::RUNNING: {
        // Still working. Check the next
      }
      break;

      case NodeStatus::SKIPPED: {
        // Node requested to be skipped or halted. Check the next
      }
      break;

      case NodeStatus::IDLE: {
        throw LogicError("[", name(), "]: A children should not return IDLE");
      }
    }
  }
  // Skip if ALL the nodes have been skipped
  return status() == (NodeStatus::RUNNING) ? NodeStatus::RUNNING : NodeStatus::SKIPPED;
}

void ParallelNode::halt()
{
  skip_list_.clear();
  ControlNode::halt();
}

size_t ParallelNode::successThreshold() const
{
  if (success_threshold_ < 0)
  {
    return size_t(std::max(int(children_nodes_.size()) + success_threshold_ + 1, 0));
  }
  else
  {
    return size_t(success_threshold_);
  }
}

size_t ParallelNode::failureThreshold() const
{
  if (failure_threshold_ < 0)
  {
    return size_t(std::max(int(children_nodes_.size()) + failure_threshold_ + 1, 0));
  }
  else
  {
    return size_t(failure_threshold_);
  }
}

void ParallelNode::setSuccessThreshold(int threshold)
{
  success_threshold_ = threshold;
}

void ParallelNode::setFailureThreshold(int threshold)
{
  failure_threshold_ = threshold;
}

}   // namespace BT
