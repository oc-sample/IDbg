//
//  mini_dump_file.h
//  IDbg
//
//  Created by mjzheng on 2021/4/13.
//  Copyright © 2021 mjzheng. All rights reserved.
//

#ifndef mini_dump_file_h
#define mini_dump_file_h

#include <stdint.h>
#include <string>

namespace IDbg {

enum DumpOptions : uint32_t {
    kHeader =  1 << 0,   // 0000 0001
    kStack =  1 << 1,   // 0000 0010
    kImage =  1 << 2,   // 0000 0100
    kFile =  1 << 3,   // 0000 1000
    kDefault = kHeader | kStack | kImage,
};

std::string GenerateMiniDump(DumpOptions options = DumpOptions::kDefault);

} // namespace IDbg

#endif /* mini_dump_file_h */
