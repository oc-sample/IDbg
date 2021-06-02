#include <string>
#include <assert.h>

#include "Windows.h"
#include "dbghelp.h"
#pragma comment(lib,"Dbghelp.lib")
bool _printCallStark(DWORD MachineType, _EXCEPTION_POINTERS * lpEP)
{
  if (!lpEP || !SymInitialize(GetCurrentProcess(), NULL, TRUE))
  {
    return false;
  }

  STACKFRAME stackFrame = { 0 };
  stackFrame.AddrPC.Mode = AddrModeFlat;
  stackFrame.AddrPC.Offset = lpEP->ContextRecord->Eip;
  stackFrame.AddrStack.Mode = AddrModeFlat;
  stackFrame.AddrStack.Offset = lpEP->ContextRecord->Esp;
  stackFrame.AddrFrame.Mode = AddrModeFlat;
  stackFrame.AddrFrame.Offset = lpEP->ContextRecord->Ebp;

  const static size_t MAX_DEPTH = 30;
  const static size_t MIN_INDEX = 3;
  size_t index = 0;
  while (StackWalk(
    MachineType, GetCurrentProcess(), GetCurrentThread(),
    &stackFrame, lpEP->ContextRecord, NULL,
    SymFunctionTableAccess, SymGetModuleBase, NULL)/*StackWalk*/)
  {
    if (index++ < MIN_INDEX) continue;
    if (index > MAX_DEPTH) break;
    assert(stackFrame.AddrFrame.Offset != 0);

    const size_t BUF_LEN = 1024;
    unsigned char * symbolBuffer = new unsigned char[sizeof(SYMBOL_INFO) + BUF_LEN];
    PSYMBOL_INFO pSymbol = reinterpret_cast<PSYMBOL_INFO>(symbolBuffer);
    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = BUF_LEN;

    SymFromAddr(GetCurrentProcess(), stackFrame.AddrPC.Offset, 0, pSymbol);

    IMAGEHLP_LINE lineInfo = { sizeof(IMAGEHLP_LINE) };
    DWORD dwLineDisplacement;
    SymGetLineFromAddr(
      GetCurrentProcess(), stackFrame.AddrPC.Offset,
      &dwLineDisplacement, &lineInfo);

    char cstrInfo[256] = { 0 };
    sprintf_s(cstrInfo, "%s(%d) : [threadID=%d] [function=%s]\n",
      lineInfo.FileName, lineInfo.LineNumber,
      GetCurrentThreadId(), pSymbol->Name);
    OutputDebugStringA(cstrInfo);

    delete[]symbolBuffer; symbolBuffer = NULL;
  }

  if (!SymCleanup(GetCurrentProcess()))
  {
    return false;
  }
  return true;
}

void printCallStark()
{
  __try
  {
    throw "";
  }
  __except (_printCallStark(
    IMAGE_FILE_MACHINE_I386,
    GetExceptionInformation())/*printCallStark*/)
  {
    printf("Some exception occures.\n");
  }
}
