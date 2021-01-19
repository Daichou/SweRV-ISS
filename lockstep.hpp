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
#include <tuple>

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

    std::vector<URV> peekAllIntRegs(const unsigned HartId);

    // API: get all CSR value
    std::vector<std::pair<URV,std::string>> peekAllCsrs(Hart<URV>& hart);

    std::vector<std::pair<URV,std::string>> peekAllCsrs(const unsigned HartId);

    // API: get all Fp register value
    std::vector<URV> peekAllFpRegs(Hart<URV>& hart);

    std::vector<URV> peekAllFpRegs(const unsigned HartId);

    // API: get specific int gpr
    URV peekIntReg(Hart<URV>& hart, const unsigned reg);

    URV peekIntReg(const unsigned HartId, const unsigned reg);

    // API: get specific float gpr
    // URV peekFpReg(Hart<URV>& hart, const unsigned reg);

    // API: get specific csr register value
    // URV peekCsr(Hart<URV>& hart, CsrNumber csr);

    // API: get pc value
    URV peekPc(Hart<URV>& hart);

    URV peekPc(const unsigned HartId);

    uint8_t peekMemory8(Hart<URV> & hart, const size_t address);
    uint32_t peekMemory32(Hart<URV> & hart, const size_t address);
    uint64_t peekMemory64(Hart<URV> & hart, const size_t address);

    uint8_t peekMemory8(const unsigned hartId, const size_t address);
    uint32_t peekMemory32(const unsigned hartId, const size_t address);
    uint64_t peekMemory64(const unsigned hartId, const size_t address);
    // API: set specific int gpr value
    bool pokeIntReg(Hart<URV>& hart, const unsigned reg, URV val);

    bool pokeIntReg(const unsigned HartId, const unsigned reg, URV val);

    // API: set specific fp gpr value
    bool pokeFpReg(Hart<URV>& hart, const unsigned reg, const uint64_t val);

    bool pokeFpReg(const unsigned HartId, const unsigned reg, const uint64_t val);

    // API: set specific csr register value
    bool pokeCsr(Hart<URV>& hart, CsrNumber csr, URV val);

    bool pokeCsr(const unsigned HartId, CsrNumber csr, URV val);

    // API: set specific PC value
    void pokePc(Hart<URV>& hart, const URV address);

    void pokePc(const unsigned HartId, const URV address);

    // API: get Current Instruction Ld/St address and value
    std::tuple<bool, URV, URV> peekCurLdSt(const unsigned HartId);

    std::tuple<bool, URV, URV> peekCurLdSt(Hart<URV>& hart);

    // API: dissassembly current instruction
    std::string disassCurrentInst(Hart<URV>&);

    std::string disassCurrentInst(const unsigned HartId);

    // API: dissassembly current instruction
    std::string disassSpecInst(Hart<URV>&, const size_t address);

    std::string disassSpecInst(const unsigned HartId, const size_t address);
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
