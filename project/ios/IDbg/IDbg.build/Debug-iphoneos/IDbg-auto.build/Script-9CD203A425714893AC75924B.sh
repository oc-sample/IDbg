#!/bin/sh
make -C /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/project/ios/IDbg -f /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/project/ios/IDbg/CMakeScripts/IDbg-auto_postBuildPhase.make$CONFIGURATION OBJDIR=$(basename "$OBJECT_FILE_DIR_normal") all
