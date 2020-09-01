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

#include <unordered_map>
#include "System.hpp"
#include "Hart.hpp"
#include <vector>
#include <utility>

namespace WdRiscv
{

  /// Manage an lockstep session. To use: Construct an instance
  /// with one or more harts then invoke the interact method which
  /// will read commands from the standard input and execute them
  /// until the quit command is seen. URV (unsigned register value) is
  /// either uint32_t or uint64_t depending on the integer register
  /// width of the harts.
  template <typename URV>
  class Lockstep
  {
  public:

    /// Constructor.
    Lockstep(System<URV>& system);

    /// Read commands from the standard input and execute them.
    /// Instance traces go the the given traceFile (no instance
    /// tracing if traceFile is NULL). Executed commands are logged to
    /// the give commandLog file (no comand logging if commandLog is
    /// NULL). Return true if all commands are executed successfully.
    /// Return false otherwise.
    bool interact(FILE* traceFile, FILE* commandLog);

    /// API: "until" command. Run until address.
    bool untilCommand(Hart<URV>&, const size_t addr,
		     FILE* traceFile);

    /// API: "step" command. Single step.
    bool stepCommand(Hart<URV>&, const size_t count,
            FILE* traceFile);

    // API: get all int gpr
    std::vector<URV> peekAllIntRegs(Hart<URV>& hart);

    // API: get all CSR value
    std::vector<std::pair<URV,std::string>> peekAllCsrs(Hart<URV>& hart);

    // API: get all Fp register value
    std::vector<URV> peekAllFpRegs(Hart<URV>& hart);

    // API: get specific int gpr
    URV peekIntReg(Hart<URV>& hart, const unsigned reg);

    // API: get specific float gpr
    // URV peekFpReg(Hart<URV>& hart, const unsigned reg);

    // API: get specific csr register value
    // URV peekCsr(Hart<URV>& hart, CsrNumber csr);

    // API: get pc value
    URV peekPc(Hart<URV>& hart);

    // API: set specific int gpr value
    bool pokeIntReg(const unsigned reg, URV val);

    // API: set specific fp gpr value
    bool pokeFpReg(const unsigned reg, const uint64_t val) const;

    // API: set specific csr register value
    bool pokeCsr(CsrNumber csr, URV val);

    // API: set specific PC value
    void pokePc(const URV address);

    // API: dissassembly current instruction
    std::string disassCurrentInst(Hart<URV>&);

    // API: Load ELF file.
    bool loadElf(Hart<URV>&, const std::string& filename);

    // API: Load HEX file.
    bool loadHex(Hart<URV>&, const std::string& filename);

    // API: Reset processor and jump to reset_pc.
    bool resetCommand(Hart<URV>&, const size_t reset_pc);

  private:

    System<URV>& system_;

    // Initial resets do not reset memory mapped registers.
    bool resetMemoryMappedRegs_ = false;
  };

}
