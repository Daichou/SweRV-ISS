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


#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "lockstep.hpp"
#include "linenoise.hpp"

using namespace WdRiscv;


/// Return format string suitable for printing an integer of type URV
/// in hexadecimal form.
template <typename URV>
static
const char*
getHexForm()
{
  if (sizeof(URV) == 4)
    return "0x%08x";
  if (sizeof(URV) == 8)
    return "0x%016x";
  if (sizeof(URV) == 16)
    return "0x%032x";
  return "0x%x";
}

template <typename URV>
Lockstep<URV>::Lockstep(System<URV>& system)
  : system_(system)
{
}


template <typename URV>
bool
Lockstep<URV>::untilCommand(Hart<URV>& hart, const size_t addr,
    FILE* traceFile)
{
  if (addr >= hart.memorySize())
    std::cerr << "Warning: Address outside memory range.\n";

  return hart.untilAddress(addr, traceFile);
}


template <typename URV>
bool
Lockstep<URV>::stepCommand(Hart<URV>& hart, const size_t count,
    FILE* traceFile)
{
  if (not hart.isStarted())
  {
    // WD special.
    std::cerr << "Cannot step a non-started hart: Consider writing "
      << "the mhartstart CSR\n";
    return false;
  }

  if (count == 0)
    return true;

  for (uint64_t i = 0; i < count; ++i)
  {
    hart.singleStep(traceFile);
    hart.clearTraceData();
  }

  return true;
}


template <typename URV>
std::vector <URV>
Lockstep<URV>::peekAllFpRegs(Hart<URV>& hart)
{
  std::vector <URV> fp_list; 
  for (unsigned i = 0; i < hart.fpRegCount(); ++i)
  {
    uint64_t val = 0;
    if (hart.peekFpReg(i, val))
      fp_list.push_back(val);
  }

  return fp_list;
}


template <typename URV>
std::vector <URV>
Lockstep<URV>::peekAllIntRegs(Hart<URV>& hart)
{
  std::vector <URV> gpr_list;

  for (unsigned i = 0; i < hart.intRegCount(); ++i)
  {
    std::string name;
    URV val = 0;
    if (hart.peekIntReg(i, val, name))
      gpr_list.push_back(val);
  }
  return gpr_list;
}


template <typename URV>
std::vector<std::pair<URV, std::string>>
Lockstep<URV>::peekAllCsrs(Hart<URV>& hart)
{
  std::vector<std::pair<URV, std::string>> csr_list;

  for (size_t i = 0; i <= size_t(CsrNumber::MAX_CSR_); ++i)
  {
    CsrNumber csr = CsrNumber(i);
    std::string name;
    URV val = 0;
    if (hart.peekCsr(csr, val, name))
      csr_list.push_back(make_pair(val, name));
  }

  return csr_list;
}

template <typename URV>
URV Lockstep<URV>::peekIntReg(Hart<URV>& hart, const unsigned reg)
{
  return hart.peekIntReg(reg);
}
/*
template <typename URV>
Lockstep<URV>::URV peekFpReg(Hart<URV>& hart, unsigned reg)
{
}

template <typename URV>
Lockstep<URV>::URV peekCsr(Hart<URV>& hart, CsrNumber csr)
{
  
}
*/
template <typename URV>
URV Lockstep<URV>::peekPc(Hart<URV>& hart)
{
  return hart.peekPc();
}

template <typename URV>
bool Lockstep<URV>::pokeIntReg(Hart<URV>& hart, unsigned reg, URV val)
{
  return hart.pokeIntReg(reg, val);
}

template <typename URV>
bool Lockstep<URV>::pokeFpReg(Hart<URV>& hart, const unsigned reg,const uint64_t val)
{
  return hart.pokeFpReg(reg, val);
}

template <typename URV>
bool Lockstep<URV>::pokeCsr(Hart<URV>& hart, CsrNumber csr, URV val)
{
  return hart.pokeCsr(csr, val);
}

template <typename URV>
void Lockstep<URV>::pokePc(Hart<URV>& hart, URV address)
{
  hart.pokePc(address);
}

template <typename URV>
std::string
Lockstep<URV>::disassCurrentInst(Hart<URV>& hart)
{
	std::string str;
	//URV pc = hart.peekPc();
  //uint32_t code;
  //hart.fetchInst(pc, code);
  //hart.disassembleInst(code, str);
	return str;
}


template <typename URV>
bool
Lockstep<URV>::loadElf(Hart<URV>& hart, const std::string& filename)
{
  size_t entryPoint = 0;

  if (not hart.loadElfFile(filename, entryPoint))
    return false;

  hart.pokePc(URV(entryPoint));

  return true;
}


template <typename URV>
bool
Lockstep<URV>::loadHex(Hart<URV>& hart, const std::string& filename)
{
  if (not hart.loadHexFile(filename))
    return false;

  return true;
}


template <typename URV>
bool
Lockstep<URV>::resetCommand(Hart<URV>& hart, const size_t reset_pc)
{
	hart.defineResetPc(reset_pc);
	hart.reset(resetMemoryMappedRegs_);
	return true;
}

template class WdRiscv::Lockstep<uint32_t>;
template class WdRiscv::Lockstep<uint64_t>;
