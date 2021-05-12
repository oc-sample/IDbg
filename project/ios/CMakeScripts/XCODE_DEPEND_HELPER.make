# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.IDbg.Debug:
/Users/mjzheng/Documents/mj_git/oc-sample/IDbg/project/ios/IDbg/Debug${EFFECTIVE_PLATFORM_NAME}/IDbg.framework/IDbg:
	/bin/rm -f /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/project/ios/IDbg/Debug${EFFECTIVE_PLATFORM_NAME}/IDbg.framework/IDbg


PostBuild.IDbg.Release:
/Users/mjzheng/Documents/mj_git/oc-sample/IDbg/project/ios/IDbg/Release${EFFECTIVE_PLATFORM_NAME}/IDbg.framework/IDbg:
	/bin/rm -f /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/project/ios/IDbg/Release${EFFECTIVE_PLATFORM_NAME}/IDbg.framework/IDbg


PostBuild.IDbg.MinSizeRel:
/Users/mjzheng/Documents/mj_git/oc-sample/IDbg/project/ios/IDbg/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/IDbg.framework/IDbg:
	/bin/rm -f /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/project/ios/IDbg/MinSizeRel${EFFECTIVE_PLATFORM_NAME}/IDbg.framework/IDbg


PostBuild.IDbg.RelWithDebInfo:
/Users/mjzheng/Documents/mj_git/oc-sample/IDbg/project/ios/IDbg/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/IDbg.framework/IDbg:
	/bin/rm -f /Users/mjzheng/Documents/mj_git/oc-sample/IDbg/project/ios/IDbg/RelWithDebInfo${EFFECTIVE_PLATFORM_NAME}/IDbg.framework/IDbg




# For each target create a dummy ruleso the target does not have to exist
