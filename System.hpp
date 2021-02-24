// Copyright 2020 Western Digital Corporation or its affiliates.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#pragma once

#include <Memory.hpp>
#include <memory>


namespace WdRiscv
{

  template <typename URV>
  class Hart;

  template <typename URV>
  class Core;


  /// Model a system consisting of n cores with m-harts per core and a
  /// memory. The harts in the system are indexed from 0 to n*m -
  /// 1. The type URV (unsigned register value) is that of the integer
  /// register and is either uint32_t or uint64_t.
  template <typename URV>
  class System
  {
  public:

    typedef Hart<URV> HartClass;
    typedef Core<URV> CoreClass;

    /// Constructor: Construct a system with n (coreCount) cores each
    /// consisting of m (hartsPerCore) harts. The harts in this system
    /// are indexed with 0 to n*m - 1.
    System(unsigned coreCount, unsigned hartsPerCore, Memory& memory);

    ~System();

    /// Return count of cores in this system.
    unsigned coreCount() const
    { return cores_.size(); }

    /// Return the number of harts per core.
    unsigned hartsPerCore() const
    { return hartsPerCore_; }

    /// Return count of harts (coreCount * hartsPerCore) in this
    /// system.
    unsigned hartCount() const
    { return hartCount_; }

    /// Return pointer to the ith hart in the system or null if i is
    /// out of bounds.
    std::shared_ptr<HartClass> ithHart(unsigned i)
    {
      if (i >= sysHarts_.size())
	return std::shared_ptr<HartClass>();
      return sysHarts_.at(i);
    }

  private:

    unsigned hartCount_;
    unsigned hartsPerCore_;

    std::vector< std::shared_ptr<CoreClass> > cores_;
    std::vector< std::shared_ptr<HartClass> > sysHarts_; // All harts in system.
  };
}
