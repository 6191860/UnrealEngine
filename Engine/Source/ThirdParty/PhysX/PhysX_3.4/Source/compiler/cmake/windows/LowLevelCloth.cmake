#
# Build LowLevelCloth
#
FIND_PACKAGE(NvSimd REQUIRED)

SET(PHYSX_SOURCE_DIR ${PROJECT_SOURCE_DIR}/../../../)

SET(LL_SOURCE_DIR ${PHYSX_SOURCE_DIR}/LowLevelCloth/src)

SET(LOWLEVELCLOTH_PLATFORM_INCLUDES
	${PHYSX_SOURCE_DIR}/LowLevelAABB/windows/include

)

SET(LLCLOTH_AVX_SOURCE
	${LL_SOURCE_DIR}/avx/SwSolveConstraints.cpp
)
SOURCE_GROUP("avx" FILES ${LLCLOTH_AVX_SOURCE})

SET(LOWLEVELCLOTH_PLATFORM_SOURCE_FILES
	${LLCLOTH_AVX_SOURCE}	
)

SET(LOWLEVELCLOTH_COMPILE_DEFS

	# Common to all configurations
	${PHYSX_WINDOWS_COMPILE_DEFS};PX_PHYSX_STATIC_LIB

	$<$<CONFIG:debug>:${PHYSX_WINDOWS_DEBUG_COMPILE_DEFS};PX_PHYSX_DLL_NAME_POSTFIX=DEBUG;>
	$<$<CONFIG:checked>:${PHYSX_WINDOWS_CHECKED_COMPILE_DEFS};PX_PHYSX_DLL_NAME_POSTFIX=CHECKED;>
	$<$<CONFIG:profile>:${PHYSX_WINDOWS_PROFILE_COMPILE_DEFS};PX_PHYSX_DLL_NAME_POSTFIX=PROFILE;>
	$<$<CONFIG:release>:${PHYSX_WINDOWS_RELEASE_COMPILE_DEFS};>
)

SET(LOWLEVELCLOTH_LIBTYPE STATIC)

# include common low level cloth settings
INCLUDE(../common/LowLevelCloth.cmake)

SET_SOURCE_FILES_PROPERTIES(${LL_SOURCE_DIR}/avx/SwSolveConstraints.cpp PROPERTIES COMPILE_FLAGS "/arch:AVX") # Removed all flags except arch, should be handled on higher level.
